// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen_ui/config.hpp>

#include<boost/python.hpp>
namespace bpy = boost::python;

#include<ramen_ui/python/pyside.hpp>
using namespace ramen_ui;

void export_ramen_ui()
{
}

// ui python module
BOOST_PYTHON_MODULE( _ramen_ui)
{
    bpy::def( "init_pyside", python::init_pyside);
}
