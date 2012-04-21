// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/python/python.hpp>

#include<ramen/nodes/graph.hpp>

namespace ramen
{

graph::graph() {}

void graph::add_node( node_ptr_t n) { nodes_.push_back( n);}

node_ptr_t graph::release_node( node_t *n)
{
    node_ptr_t result;
    node_container_type::iterator it;

    for( it = nodes_.begin(); it != nodes_.end(); ++it)
    {
        if( it->get() == n)
        {
            result = *it;
            break;
        }
    }

    if( it != nodes_.end())
        nodes_.erase( it);

    return result;
}

void graph::add_edge( const edge_t& e)
{
    node_t *src = e.dst->input_plugs()[e.port].input();

    if( src)
        remove_edge( edge_t( src, e.dst, e.port));

    e.dst->input_plugs()[e.port].set_input( e.src);
    e.src->output_plug().add_output( e.dst, e.port);
    edges_.push_back( e);
}

void graph::remove_edge( const edge_t& e)
{
    e.src->output_plug().remove_output( e.dst, e.port);
    e.dst->input_plugs()[e.port].clear_input();
    edges_.erase( std::find( edges_.begin(), edges_.end(), e));
}

// connections
void graph::connect( node_t *src, node_t *dst, int port)        { add_edge( edge_t( src, dst, port));}
void graph::disconnect( node_t *src, node_t *dst, int port)     { remove_edge( edge_t( src, dst, port));}

} // namespace
