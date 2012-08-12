// Copyright (c) 2012 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/config.hpp>

#include<ramen/python/python.hpp>

#include<boost/python.hpp>

#include<ramen/nodes/node.hpp>
#include<ramen/nodes/composite_node.hpp>

#include<ramen/python/read_only_vector_proxies.hpp>

namespace bpy = boost::python;
using namespace ramen;
using namespace ramen::nodes;

namespace
{

python::read_only_ptr_vector_proxy_t<node_t> node_get_child_nodes( node_t *n)
{
    if( composite_node_t *cn = dynamic_cast<composite_node_t*>( n))
    {
        boost::ptr_vector<node_t>& children( cn->nodes());
        return python::read_only_ptr_vector_proxy_t<node_t>( &children);
    }

    return python::read_only_ptr_vector_proxy_t<node_t>( 0);
}

} // unnamed

void export_node()
{
    bpy::class_<node_t, bpy::bases<params::parameterised_t>, boost::noncopyable>( "node", bpy::no_init)
        .def( "nodes", node_get_child_nodes)
        ;

    python::read_only_ptr_vector_proxy_t<node_t>::bind( "read_only_node_ptr_vector_proxy");
}
