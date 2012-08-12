// Copyright (c) 2012 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/config.hpp>

#include<ramen/python/python.hpp>

#include<boost/python.hpp>

#include<ramen/nodes/world_node.hpp>

namespace bpy = boost::python;
using namespace ramen;
using namespace ramen::nodes;

namespace
{

} // unnamed

void export_world_node()
{
    bpy::class_<world_node_t, bpy::bases<composite_node_t>, boost::noncopyable>( "world_node", bpy::no_init)
        .def( "find_node", (node_t* (world_node_t::*)( const std::string&)) &world_node_t::find_node, bpy::return_value_policy<bpy::reference_existing_object>())
        ;
}
