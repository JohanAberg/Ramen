// Copyright (c) 2012 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/config.hpp>

#include<ramen/python/python.hpp>

#include<boost/python.hpp>

#include<ramen/manipulators/manipulable.hpp>

namespace bpy = boost::python;
using namespace ramen;

namespace
{

} // unnamed

void export_manipulable()
{
    bpy::class_<manipulators::manipulable_t, boost::noncopyable>( "manipulable", bpy::no_init)
        ;
}
