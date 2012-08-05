// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/python/python.hpp>

#include<ramen/nodes/graph.hpp>

#include<boost/foreach.hpp>
#include<boost/range/algorithm/find.hpp>

#include<base/container/ptr_vector_util.hpp>

#include<ramen/assert.hpp>

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

void graph_t::add_node( std::auto_ptr<node_t> n)
{
    nodes_.push_back( n);
}

std::auto_ptr<node_t> graph_t::release_node( node_t *n)
{
    RAMEN_ASSERT( base::container::contains_ptr( n, nodes_));

    return base::container::release_ptr( n, nodes_);
}

void graph_t::add_connection( const connection_type& c)
{
    RAMEN_ASSERT( c.src);
    RAMEN_ASSERT( c.dst);
    RAMEN_ASSERT( base::container::contains_ptr( c.src, nodes_));
    RAMEN_ASSERT( base::container::contains_ptr( c.dst, nodes_));
    RAMEN_ASSERT( c.dst->find_input( c.dst_plug) != -1);
    RAMEN_ASSERT( c.src->find_output( c.src_plug) != -1);

    boost::optional<connection_type> old_c = find_connection( c.dst, c.dst_plug);

    if( old_c)
        remove_connection( old_c.get());

    c.dst->input_plug( c.dst_plug).set_input( c.src, c.src_plug);
    c.src->output_plug( c.src_plug).add_output( c.dst, c.dst_plug);
}

void graph_t::remove_connection( const connection_type& c)
{
    RAMEN_ASSERT( c.src);
    RAMEN_ASSERT( c.dst);
    RAMEN_ASSERT( base::container::contains_ptr( c.src, nodes_));
    RAMEN_ASSERT( base::container::contains_ptr( c.dst, nodes_));
    RAMEN_ASSERT( c.dst->find_input( c.dst_plug) != -1);
    RAMEN_ASSERT( c.src->find_output( c.src_plug) != -1);

    connection_iterator it = boost::range::find( connections(), c);

    if( it != connections().end())
    {
        c.dst->input_plug( c.dst_plug).clear_input();
        c.src->output_plug( c.src_plug).remove_output( c.dst, c.dst_plug);
        connections().erase( it);
    }
}

boost::optional<graph_t::connection_type> graph_t::find_connection( node_t *dst, const base::name_t& dst_plug) const
{
    RAMEN_ASSERT( dst);
    RAMEN_ASSERT( base::container::contains_ptr( dst, nodes_));
    RAMEN_ASSERT( dst->find_input( dst_plug) != -1);

    BOOST_FOREACH( const connection_type& c, connections())
    {
        if( c.dst == dst && c.dst_plug == dst_plug)
            return c;
    }

    boost::optional<connection_type>();
}

} // namespace
} // namespace
