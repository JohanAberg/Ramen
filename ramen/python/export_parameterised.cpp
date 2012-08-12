// Copyright (c) 2012 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/config.hpp>

#include<ramen/python/python.hpp>

#include<boost/python.hpp>

#include<ramen/params/parameterised.hpp>

namespace bpy = boost::python;
using namespace ramen;
using namespace ramen::params;

namespace
{

} // unnamed

void export_parameterised()
{
    bpy::class_<parameterised_t, bpy::bases<manipulators::manipulable_t>, boost::noncopyable>( "parameterised", bpy::no_init)
        .def( "name", &parameterised_t::name, bpy::return_value_policy<bpy::copy_const_reference>())
        ;
}
