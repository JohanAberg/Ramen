// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen_ui/config.hpp>

#include<boost/python.hpp>
namespace bpy = boost::python;

void export_splash_screen();
void export_user_interface();

// ui python module
BOOST_PYTHON_MODULE( _ramen_ui)
{
    export_splash_screen();
    export_user_interface();
}
