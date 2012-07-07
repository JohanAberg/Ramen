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

graph_t::connection_type::connection_type() : src( 0), dst( 0) {}

graph_t::connection_type::connection_type( node_t *s, const base::name_t& splug, node_t *d, const base::name_t& dplug) : src( s),
                                                                                                        src_plug( splug),
                                                                                                        dst( d),
                                                                                                        dst_plug( dplug)
{
    RAMEN_ASSERT( s);
    RAMEN_ASSERT( d);
}

bool graph_t::connection_type::operator==( const connection_type& other) const
{
    return src == other.src && dst == other.dst &&
            src_plug == other.src_plug &&
            dst_plug == other.dst_plug;
}

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

void graph_t::add_connection( const connection_type& e)
{
    // TODO: implement this.
    /*
    node_t *src = e.dst->input_plugs()[e.port].input_node();

    if( src)
        remove_connection( connection_type( src, e.dst, e.port));

    e.dst->input_plugs()[e.port].set_input( e.src);
    e.src->output_plug().add_output( e.dst, e.port);
    connections_.push_back( e);
    */
}

void graph_t::remove_connection( const connection_type& e)
{
    // TODO: implement this.
    /*
    e.src->output_plug().remove_output( e.dst, e.port);
    e.dst->input_plugs()[e.port].clear_input();
    connections_.erase( std::find( connections_.begin(), connections_.end(), e));
    */
}

} // namespace
} // namespace
