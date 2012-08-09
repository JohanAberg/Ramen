// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen_ui/config.hpp>

#include<boost/python.hpp>
namespace bpy = boost::python;

#include<ramen_ui/splash_screen.hpp>
using namespace ramen_ui;

namespace
{

void finish( splash_screen_t *splash, long widget)
{
    splash->finish( reinterpret_cast<QWidget*>( widget));
}

} // unnamed

void export_splash_screen()
{
    bpy::class_<splash_screen_t, boost::noncopyable>( "splash_screen")
        .def( "show", &splash_screen_t::show)
        .def( "show_message", &splash_screen_t::show_message)
        .def( "finish", finish)
        ;
}
