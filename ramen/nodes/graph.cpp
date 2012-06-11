// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/python/python.hpp>

#include<ramen/nodes/graph.hpp>

#include<ramen/assert.hpp>

#include<ramen/container/ptr_vector_util.hpp>

namespace ramen
{
namespace nodes
{

graph_t::graph_t() {}

graph_t::graph_t( const graph_t& other)
{
    // TODO: implement this. (est.)
    RAMEN_ASSERT( false);
}

void graph_t::add_node( std::auto_ptr<node_t> n) { nodes_.push_back( n);}

std::auto_ptr<node_t> graph_t::release_node( node_t *n)
{
    return container::release_ptr( n, nodes_);
}

void graph_t::add_edge( const edge_t& e)
{
    node_t *src = e.dst->input_plugs()[e.port].input_node();

    if( src)
        remove_edge( edge_t( src, e.dst, e.port));

    e.dst->input_plugs()[e.port].set_input( e.src);
    e.src->output_plug().add_output( e.dst, e.port);
    edges_.push_back( e);
}

void graph_t::remove_edge( const edge_t& e)
{
    e.src->output_plug().remove_output( e.dst, e.port);
    e.dst->input_plugs()[e.port].clear_input();
    edges_.erase( std::find( edges_.begin(), edges_.end(), e));
}

// connections
void graph_t::connect( node_t *src, node_t *dst, int port)        { add_edge( edge_t( src, dst, port));}
void graph_t::disconnect( node_t *src, node_t *dst, int port)     { remove_edge( edge_t( src, dst, port));}

} // namespace
} // namespace
