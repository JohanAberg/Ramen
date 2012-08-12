// Copyright (c) 2012 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/config.hpp>

#include<ramen/python/python.hpp>

#include<boost/python.hpp>

#include<ramen/nodes/composite_node.hpp>

namespace bpy = boost::python;
using namespace ramen;

namespace
{

} // unnamed

void export_composite_node()
{
    bpy::class_<nodes::composite_node_t, bpy::bases<nodes::node_t>, boost::noncopyable>( "composite_node", bpy::no_init)
        ;
}
