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

#include<ramen/version.hpp>

#include<ramen/app/document.hpp>
#include<ramen/app/plugin_manager.hpp>
#include<ramen/app/preferences.hpp>

#include<ramen/memory/manager.hpp>
#include<ramen/nodes/factory.hpp>
#include<ramen/ocio/manager.hpp>
#include<ramen/undo/stack.hpp>

#include<ramen_ui/user_interface.hpp>

// tests
int run_ramen_unit_tests( int argc, char **argv);

namespace ramen
{

application_t *g_app = 0;

application_t::application_t()
{
    RAMEN_ASSERT( g_app == 0 );
    g_app = this;
    ui_ = 0;
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
    //boost::filesystem::create_directories( preferences().tmp_dir());
}

void application_t::init_threads( int num)
{
    if( num == 0)
        max_threads_ = boost::thread::hardware_concurrency();
    else
        max_threads_ = num;

    task_scheduler_.initialize( max_threads_);
    Imf::setGlobalThreadCount( max_threads_);
}

void application_t::init_ocio()
{
    ocio_manager_.reset( new ocio::manager_t());
}

void application_t::set_ui(ramen_ui::user_interface_t *ui)
{
    RAMEN_ASSERT( !ui_);
    RAMEN_ASSERT( ui);

    ui_ = ui;
}

const preferences_t& application_t::preferences() const
{
    RAMEN_ASSERT( preferences_.get());
    return *preferences_;
}

preferences_t &application_t::preferences()
{
    RAMEN_ASSERT( preferences_.get());
    return *preferences_;
}

void application_t::set_preferences( boost::python::object prefs)
{
    RAMEN_ASSERT( !preferences_.get());
    preferences_.reset( new preferences_t( prefs));
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
    //std::cout << "Image Cache Memory = " << mem_manager_->image_allocator().max_size() / 1024 / 1024 << " Mb\n";
    std::cout << std::endl;
}

const document_t& application_t::document() const
{
    RAMEN_ASSERT( document_.get());
    return *document_;
}

document_t& application_t::document()
{
    RAMEN_ASSERT( document_.get());
    return *document_;
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
}

// messages
void application_t::fatal_error( const std::string& message, bool no_gui) const
{
    if( ui() && !no_gui)
        ui()->fatal_error( message);
    else
    {
        std::cerr << "Fatal error: " << message << "\n";
        abort();
    }
}

void application_t::error( const std::string& message, bool no_gui) const
{
    if( ui() && !no_gui)
        ui()->error( message);
    else
    {
        std::cerr << "Error: " << message << "\n";
    }
}

void application_t::inform( const std::string& message, bool no_gui) const
{
    if( ui() && !no_gui)
        ui()->inform( message);
    else
    {
        std::cerr << "Info: " << message << "\n";
    }
}

bool application_t::question( const std::string& what, bool default_answer) const
{
    if( ui())
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

bool application_t::debug_build() const
{
    #ifndef NDEBUG
        return true;
    #endif

    return false;
}

int application_t::run_unit_tests( int argc, char **argv)
{
    return run_ramen_unit_tests( argc, argv);
}

application_t& app()
{
    RAMEN_ASSERT( g_app);
    return *g_app;
}

} // namespace
