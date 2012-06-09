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

#include<glog/logging.h>

#include<ramen/version.hpp>

#include<ramen/app/preferences.hpp>
#include<ramen/app/document.hpp>
#include<ramen/app/plugin_manager.hpp>

#include<ramen/memory/manager.hpp>

#include<ramen/nodes/node_factory.hpp>

#include<ramen/ocio/manager.hpp>

#include<ramen/undo/stack.hpp>

#include<ramen/util/command_line_parser.hpp>

#include<ramen/ui/user_interface.hpp>
#include<ramen/ui/main_window.hpp>
#include<ramen/ui/dialogs/splash_screen.hpp>

// Tests
#ifndef NDEBUG
    int run_ramen_unit_tests( int argc, char **argv);
#endif

namespace ramen
{

application_t *g_app = 0;

application_t::application_t( int argc, char **argv) : system_(), preferences_()
{
    RAMEN_ASSERT( g_app == 0 );
    g_app = this;

    max_threads_ = 0;
    img_cache_size_ = 0;
    command_line_ = false;
    quitting_ = false;

    #ifndef NDEBUG
        run_unit_tests_ = false;
    #endif

    cmd_parser_.reset( new util::command_line_parser_t( argc, argv));

    google::InitGoogleLogging( cmd_parser_->argv[0]);
    //google::SetLogDestination( google::INFO, "filename.log");

    // Create QApplication
    QApplication *q_app = new QApplication( cmd_parser_->argc, cmd_parser_->argv);
    boost::filesystem::path bundle_path( system().app_bundle_path());
    bundle_path /= "lib/Qt_plugins";
    qApp->setLibraryPaths( QStringList( QString( ramen::filesystem::file_cstring( bundle_path))));

    parse_command_line( cmd_parser_->argc, cmd_parser_->argv);

    if( !system().simd_type() & system::simd_sse2)
        fatal_error( "No SSE2 instruction set, exiting", true);

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
    node_factory_t::instance();

    if( !command_line_)
        splash_->show_message( "Loading plugins...");
    plugin_manager_t::instance();

    if( !command_line_)
        splash_->show_message( "Initializing OpenColorIO");
    ocio_manager_.reset( new ocio::manager_t());

    if( !command_line_)
    {
        splash_->show_message( "Initializing user interface");
        ui_.reset( new ui::user_interface_t());
        ui_->init();
        print_app_info();
    }
}

application_t::~application_t()
{
    //	TODO: implement this.
    //delete_tmp_files();

    google::ShutdownGoogleLogging();
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
        ui()->show();
        splash_->finish( ui()->main_window());
        splash_.reset();
        return ui()->run();
    }

    #ifndef NDEBUG
        if( run_unit_tests_)
        {
            int result = run_ramen_unit_tests( cmd_parser_->argc, cmd_parser_->argv);
            std::exit( result);
        }
    #endif

    return 0;
}

// command line things
bool application_t::matches_option( char *arg, const char *opt) const
{
    if( !strcmp( arg, opt))
        return true;

    return false;
}

void application_t::parse_command_line( int argc, char **argv)
{
    if( argc == 1)
        return;

    if( matches_option( argv[1], "-help"))
        usage();

    if( matches_option( argv[1], "-version"))
    {
        std::cout << RAMEN_NAME_FULL_VERSION_STR << ", " << __DATE__ << std::endl;
        std::exit( 0);
    }

    #ifndef NDEBUG
        if( matches_option( argv[1], "-runtests"))
        {
            run_unit_tests_ = true;
            command_line_ = true;
            return;
        }
    #endif
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
    std::exit( 0);
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

// document handling
void application_t::create_new_document()
{
    delete_document();
    document_.reset( new document_t());
}

void application_t::open_document( const boost::filesystem::path& p)
{
    create_new_document();
    /*
    boost::filesystem::ifstream ifs( p, serialization::yaml_iarchive_t::file_open_mode());

    if( !ifs.is_open() || !ifs.good())
        throw std::runtime_error( std::string( "Couldn't open input file ") + filesystem::file_string( p));

    std::auto_ptr<serialization::yaml_iarchive_t> in;
    in.reset( new serialization::yaml_iarchive_t( ifs));

    if( !in->read_composition_header())
        throw std::runtime_error( std::string( "Couldn't read file header ") + filesystem::file_string( p));

    document().set_file( p);
    document().load( *in);

    std::string err = in->errors();

    if( !err.empty())
    {
        // TODO: display errors here
        // multiline_alert_t::Instance().show_alert( "Errors during file open", err);
    }
    */
}

void application_t::delete_document()
{
    document_.reset( 0);
    memory_manager().clear_caches();
}

// messages
void application_t::fatal_error( const std::string& message, bool no_gui) const
{
    if( !command_line_ && ui() && !ui()->rendering() && !no_gui)
        ui()->fatal_error( message);
    else
    {
        std::cerr << "Fatal error: " << message << "\n";
        DLOG( FATAL) << message;
        abort();
    }
}

void application_t::error( const std::string& message, bool no_gui) const
{
    if( !command_line_ && ui() && !ui()->rendering() && !no_gui)
        ui()->error( message);
    else
    {
        std::cerr << "Error: " << message << "\n";
        DLOG( ERROR) << message;
    }
}

void application_t::inform( const std::string& message, bool no_gui) const
{
    if( !command_line_ && ui() && !ui()->rendering() && !no_gui)
        ui()->inform( message);
    else
    {
        std::cerr << "Info: " << message << "\n";
        DLOG( INFO) << message;
    }
}

bool application_t::question( const std::string& what, bool default_answer) const
{
    if( !command_line_ && ui() && !ui()->rendering())
        return ui()->question( what, default_answer);
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
