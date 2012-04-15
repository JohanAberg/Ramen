// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/app/application.hpp>

#include<cstdlib>
#include<cstring>

#include<exception>
#include<algorithm>
#include<iostream>
#include<fstream>

#include<boost/thread.hpp>
#include<boost/filesystem/operations.hpp>
#include<boost/filesystem/convenience.hpp>
#include<boost/lexical_cast.hpp>

#include<OpenEXR/ImfThreading.h>

#include<QApplication>

#include<ramen/version.hpp>

#include<ramen/app/preferences.hpp>
#include<ramen/app/document.hpp>
#include<ramen/app/plugin_manager.hpp>

#include<ramen/nodes/node_factory.hpp>

#include<ramen/image/init_image_processing.hpp>
#include<ramen/movieio/factory.hpp>

#include<ramen/render/render_thread.hpp>
#include<ramen/render/render_sequence.hpp>

#include<ramen/undo/stack.hpp>

#include<ramen/ui/user_interface.hpp>
#include<ramen/ui/main_window.hpp>
#include<ramen/ui/dialogs/splash_screen.hpp>

#include<ramen/python/interpreter.hpp>

#ifndef NDEBUG
	#include<ramen/test/test.hpp>
#endif

namespace ramen
{

application_t *g_app = 0;

application_t::application_t( int argc, char **argv) : system_(),
                                                       preferences_()
{
    RAMEN_ASSERT( g_app == 0 );
    g_app = this;

    max_threads_ = 0;
    img_cache_size_ = 0;
    command_line_ = false;
	render_mode_ = false;    

    QApplication *q_app = new QApplication( argc, argv);
    boost::filesystem::path bundle_path( system().app_bundle_path());
    bundle_path /= "lib/Qt_plugins";
    qApp->setLibraryPaths( QStringList( QString( ramen::filesystem::file_cstring( bundle_path))));

    parse_command_line( argc, argv);

    int simd = system().simd_type();

    if( !simd & system::simd_sse2)
    {
        std::cout << "No SSE2 instructions, exiting\n";
        exit( 0);
    }

    // init prefs
    preferences_.reset( new preferences_t());

    create_dirs();

    if( max_threads_ == 0)
        max_threads_ = boost::thread::hardware_concurrency();

    task_scheduler_.initialize( max_threads_);
    Imf::setGlobalThreadCount( max_threads_);

    if( !command_line_)
    {
        splash_.reset( new ui::splash_screen_t());
        splash_->show();
        splash_->show_message( RAMEN_NAME_VERSION_STR);
    }

    // init memory manager
    if( img_cache_size_ == 0)
    {
        boost::uint64_t percent  = preferences().max_image_memory();
        boost::uint64_t ram_size = system().ram_size();
        img_cache_size_ = ram_size / (boost::uint64_t) 100 * percent;
    }

    mem_manager_.reset( new memory::manager_t( img_cache_size_));

    if( !command_line_)
        splash_->show_message( "Initializing builtin nodes");
    node_factory_t::Instance();

    if( !command_line_)
        splash_->show_message( "Loading plugins...");
    plugin_manager_t::Instance();

    if( !command_line_)
        splash_->show_message( "Initializing image processing");
    image::init_image_processing();

    if( !command_line_)
        splash_->show_message( "Initializing movieio");
    movieio::factory_t::Instance().init();

    if( !command_line_)
        splash_->show_message( "Initializing OpenColorIO");
    init_ocio();

    if( !command_line_)
        splash_->show_message( "Initializing Python");
    python::interpreter_t::Instance();

    if( !command_line_)
        splash_->show_message( "Initializing render thread");
    render_thread_.init();

    if( !command_line_)
    {
        splash_->show_message( "Initializing user interface");
        ui::user_interface_t::Instance().init();
        print_app_info();
    }
}

application_t::~application_t()
{ 
	//	TODO: implement this.
	//delete_tmp_files();
}

void application_t::create_dirs()
{
	// home
    boost::filesystem::path base( system().app_user_path());
    boost::filesystem::create_directories( base / "ocio");
    boost::filesystem::create_directories( base / "prefs");
    boost::filesystem::create_directories( base / "python");
    boost::filesystem::create_directories( base / "ui");

	// tmp
    boost::filesystem::create_directories( preferences().tmp_dir());
}

int application_t::run()
{
    if( !command_line_)
    {
        ui::user_interface_t::Instance().show();
        splash_->finish( ui::user_interface_t::Instance().main_window());
        splash_.reset();
        return ui::user_interface_t::Instance().run( infile_);
    }
    else
	{
		if( render_mode_)
		{
			try
			{
				open_document( infile_);
			}
			catch( std::exception& e)
			{
				fatal_error( e.what(), true);
			}
			
			if( !start_frame_)
				start_frame_ = document_t::Instance().composition().start_frame();
			
			if( !end_frame_)
				end_frame_ = document_t::Instance().composition().end_frame();

			if( !proxy_level_)
				proxy_level_ = 0;

			if( !subsample_)
				subsample_ = 1;
			
			if( !mb_extra_samples_)
				mb_extra_samples_ = 0;
			
			if( !mb_shutter_factor_)
				mb_shutter_factor_ = 1.0f;

			render::render_sequence( document_t::Instance().composition(), start_frame_.get(), end_frame_.get(),
									 proxy_level_.get(), subsample_.get(), mb_extra_samples_.get(), mb_shutter_factor_.get());
		}
	}

    return 0;
}

// command line things
bool application_t::matches_option( char *arg, const char *opt) const
{
	if( !strcmp( arg, opt))
		return true;
	
	return false;
}

boost::optional<int> application_t::parse_int( int num, int argc, char **argv) const
{
	RAMEN_ASSERT( num > 0);
	RAMEN_ASSERT( argc > 0);
	
	boost::optional<int> result;
	
	if( num < argc)
	{
		try 
		{ 
			result = boost::lexical_cast<int>( argv[num]);
		}
		catch( std::exception& e) {}
	}
	
	return result;
}

boost::optional<float> application_t::parse_float( int num, int argc, char **argv) const
{
	RAMEN_ASSERT( num > 0);
	RAMEN_ASSERT( argc > 0);
	
	boost::optional<float> result;
	
	if( num < argc)
	{
		try 
		{ 
			result = boost::lexical_cast<float>( argv[num]);
		}
		catch( std::exception& e) {}
	}
	
	return result;
}

void application_t::parse_input_file( char *arg)
{
	// input file name
	infile_ = boost::filesystem::path( arg);
	
	if( infile_.is_relative())
		infile_ = boost::filesystem::absolute( infile_);
}

bool application_t::parse_common_option( int argc, char **argv, int& num)
{
	if( matches_option( argv[num], "-threads"))
	{
		boost::optional<int> op( parse_int( num + 1, argc, argv));
		
		if( op)
		{
			if( op.get() > 0)
				max_threads_ = op.get();
			else
				error( "threads should be equal or greater than 1. Ignoring", true);
		}
		else
		{
			fatal_error( "no number of threads given.", true);
			return false;
		}

		num += 2;
		return true;
	}
	else
	{
		if( argv[num][0] == '-')
			fatal_error( std::string( "unknown option: ") + argv[num], true);
	}
	
	return false;
}

void application_t::parse_command_line( int argc, char **argv)
{
	if( argc == 1)
		return;

	if( matches_option( argv[1], "-help") || matches_option( argv[1], "-h"))
		usage();
	
	if( matches_option( argv[1], "-version"))
	{
		std::cout << RAMEN_NAME_FULL_VERSION_STR << ", " << __DATE__ << std::endl;
		exit( 0);
	}

	#ifndef NDEBUG
		if( matches_option( argv[1], "-runtests"))
		{
			int result = run_unit_tests( argc, argv);
			exit( result);
		}
	#endif
	
    int i = 1;
	
    while (i < argc)
    {
		if( matches_option( argv[i], "-render"))
		{
			command_line_ = true;
			render_mode_ = true;
			++i;
			parse_render_command_line( argc, argv, i);
			return;
		}
		else if( parse_common_option( argc, argv, i))
			;
		else
		{
			parse_input_file( argv[i]);
			++i;
		}
	}
}

void application_t::parse_render_command_line( int argc, char **argv, int num)
{
    int i = num;
    while (i < argc)
    {
		if( matches_option( argv[i], "-help") || matches_option( argv[i], "-h"))
			render_usage();
		else if( matches_option( argv[i], "-frames"))
		{		
			start_frame_ = parse_int( i + 1, argc, argv);
			end_frame_ = parse_int( i + 2, argc, argv);
			
			if( !start_frame_ || !end_frame_)
				render_usage();
			
			i += 3;
		}
		// TODO: add more options here...
		else if( parse_common_option( argc, argv, i))
		{
		}
		else
		{
			parse_input_file( argv[i]);
			++i;
		}		
	}
	
	if( infile_.empty())
		fatal_error( "No composition file given", true);

	// TODO: check everything needed here!
}

void application_t::usage()
{
    std::cout <<	RAMEN_NAME_FULL_VERSION_STR << ", " << __DATE__ << "\n" <<
					"Usage: ramen [options] file...\n\n"
					"Options:\n"
					"-help, -h:       Print help message and exit.\n"
					"-version:        Print version number and exit.\n"
					"-threads n:      Use n threads.\n\n"
					"-render:         Render composition. Run ramen -render -help for more information.\n"
					
					#ifndef NDEBUG
					"-runtests:       Run unit tests and exit.\n"
					#endif
					
					<< std::endl;
    exit( 0);
}

void application_t::render_usage()
{
    std::cout <<	RAMEN_NAME_FULL_VERSION_STR << ", " << __DATE__ << "\n" <<
					"Usage: ramen -render [options] file...\n\n"
					"Options:\n"
					"-help, -h:       Print this help message and exit.\n"
					"-threads n:      Use n threads.\n\n"
					"-frames n m:     Render frames n to m.\n"
					<< std::endl;
    exit( 0);
}

void application_t::print_app_info()
{
    std::cout << RAMEN_NAME_FULL_VERSION_STR << ", " << __DATE__ << std::endl;
    std::cout << "System = " << system().system_name() << std::endl;

    switch( system().cpu_type())
    {
		case system::cpu_x86:
		{
			std::cout << "CPU = Intel compatible\n";
	
			#ifdef RAMEN_SSE_SUPPORT
				int sset = system().simd_type();
	
				if( sset == system::simd_none)
					std::cout << "SIMD = None\n";
				else
				{
					std::cout << "SIMD = SSE ";
	
					if( sset & system::simd_sse2)
						std::cout << "SSE2 ";
	
					if( sset & system::simd_sse3)
						std::cout << "SSE3 ";
	
					std::cout << std::endl;
				}
			#endif
		}
		break;
	
		default:
			std::cout << "CPU = Unknown\n";
    }

    std::cout << "Using " << max_threads_ << " threads\n";
    std::cout << "Ram Size = " << system().ram_size() / 1024 / 1024 << " Mb\n";
    std::cout << "Image Cache Memory = " << mem_manager_->image_allocator().max_size() / 1024 / 1024 << " Mb\n";
}

// ocio
void application_t::init_ocio()
{
	if( getenv( "OCIO"))
	{
		try
		{
		    OCIO::ConstConfigRcPtr config = OCIO::GetCurrentConfig();
			return;
		}
		catch( OCIO::Exception & exception)
		{
			#ifndef NDEBUG
				std::cout << "Couldn't read $OCIO config. Trying Ramen's default config.";
			#endif
		}
	}
	
	// try more paths here...
	boost::filesystem::path ocio_path = system().app_user_path() / "ocio/config.ocio";

	if( init_ocio_config_from_file( ocio_path))
		return;
	
	ocio_path = system().app_bundle_path() / "ocio/config.ocio";
	
	if( init_ocio_config_from_file( ocio_path))
		return;

	// Fallback
	OCIO::ConstConfigRcPtr config = OCIO::GetCurrentConfig();
}

bool application_t::init_ocio_config_from_file( const boost::filesystem::path& p)
{
	if( !boost::filesystem::exists( p))
		return false;
	
	try
	{
		OCIO::ConstConfigRcPtr config = OCIO::Config::CreateFromFile( filesystem::file_string( p).c_str());
		OCIO::SetCurrentConfig( config);
		return true;
	}
	catch( ...)
	{
		return false;
	}
}

OCIO::ConstConfigRcPtr application_t::current_ocio_config() const
{
	OCIO::ConstConfigRcPtr config = OCIO::GetCurrentConfig();
	config->sanityCheck();
	return config;
}

// document handling
void application_t::create_new_document()
{
    undo::stack_t::Instance().clear();
    Loki::DeletableSingleton<document_impl>::GracefulDelete();
    memory_manager().clear_caches();
}

void application_t::open_document( const boost::filesystem::path& p)
{
    create_new_document();	
    boost::filesystem::ifstream ifs( p, serialization::yaml_iarchive_t::file_open_mode());

    if( !ifs.is_open() || !ifs.good())
		throw std::runtime_error( std::string( "Couldn't open input file ") + filesystem::file_string( p));

	std::auto_ptr<serialization::yaml_iarchive_t> in;
	in.reset( new serialization::yaml_iarchive_t( ifs));
		
	if( !in->read_composition_header())
		throw std::runtime_error( std::string( "Couldn't read file header ") + filesystem::file_string( p));
		
	document_t::Instance().set_file( p);
	document_t::Instance().load( *in);

	std::string err = in->errors();
	
	if( !err.empty())
	{
		// TODO: display errors here
		// multiline_alert_t::Instance().show_alert( "Errors during file open", err);
	}
}

// messages
void application_t::fatal_error( const std::string& message, bool no_gui) const
{
    if( !command_line_ && !ui::user_interface_t::Instance().rendering() && !no_gui)
        ui::user_interface_t::Instance().fatal_error( message);
    else
        std::cerr << "Fatal error: " << message << "\n";

    abort();
}

void application_t::error( const std::string& message, bool no_gui) const
{
    if( !command_line_ && !ui::user_interface_t::Instance().rendering() && !no_gui)
        ui::user_interface_t::Instance().error( message);
    else
        std::cerr << "Error: " << message << "\n";
}

void application_t::inform( const std::string& message, bool no_gui) const
{
    if( !command_line_ && !ui::user_interface_t::Instance().rendering() && !no_gui)
        ui::user_interface_t::Instance().inform( message);
    else
        std::cerr << "Info: " << message << "\n";
}

bool application_t::question( const std::string& what, bool default_answer) const
{
    if( !command_line_ && !ui::user_interface_t::Instance().rendering())
        return ui::user_interface_t::Instance().question( what, default_answer);
    else
    {
        if( default_answer)
            std::cout << "Ramen, question: " << what << ", replying yes by default\n";
        else
            std::cout << "Ramen, question: " << what << ", replying no by default\n";
    }
	
	return default_answer;
}

application_t& app()
{
    RAMEN_ASSERT( g_app);
    return *g_app;
}

} // namespace
