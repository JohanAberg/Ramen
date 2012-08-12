// Copyright (c) 2012 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/config.hpp>

#include<ramen/python/python.hpp>

#include<boost/python.hpp>

#include<ramen/params/param.hpp>

namespace bpy = boost::python;
using namespace ramen;

namespace
{

} // unnamed

void export_param()
{
    bpy::class_<params::param_t, boost::noncopyable>( "param", bpy::no_init)
        ;
}
