// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/python/python.hpp>

#include<stdlib.h>
#include<string.h>

#include<iostream>
#include<exception>

#include<glog/logging.h>

#include "client/linux/handler/exception_handler.h" // breakpad

#include<ramen/app/application.hpp>
#include<ramen/python/interpreter.hpp>
#include<ramen/filesystem/path.hpp>

namespace bfs = boost::filesystem;

// crash handling
bool ramen_crash_dump_callback( const char* dump_path,
                                  const char* minidump_id,
                                  void* context,
                                  bool succeeded)
{
    /*
    char command_string[1024];

    snprintf( command_string, sizeof( command_string ),
              "./CrashReporter ExampleClient 0.01 Linux-64 %s/%s.dmp", dump_path, minidump_id );

    int unused = system( command_string );
    */
    return succeeded;
}

void ramen_terminate( void)
{
	ramen::app().fatal_error( "Ramen has encountered a fatal error");
}
	
void ramen_unexpected( void)
{
	ramen::app().error( "Ramen has encountered an unexpected exception");
	throw std::runtime_error( "Ramen has encountered an unexpected exception.\nPlease report this bug." );
}

int main( int argc, char **argv)
{
    google_breakpad::ExceptionHandler eh( "/tmp", NULL, ramen_crash_dump_callback, NULL, true );

	std::set_terminate( &ramen_terminate);
	std::set_unexpected( &ramen_unexpected);
	
	try
	{
        int result =ramen::python::interpreter_t::instance().run_main( argc, argv);
        return result;
	}
	catch( boost::python::error_already_set)
	{
		PyErr_Print();
		PyErr_Clear();
	}
	catch( std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
        DLOG( ERROR) << "Exception: " << e.what() << "\n";
    }
	catch( ...)
	{
		std::cerr << "Unknown exception" << std::endl;
        DLOG( ERROR) << "Unknown exception" << std::endl;
    }
}
