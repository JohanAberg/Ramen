// Copyright (c) 2012 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/python/python.hpp>

#include<ramen/dependency/graph.hpp>

#include<boost/foreach.hpp>

#include<ramen/assert.hpp>

#include<ramen/nodes/world_node.hpp>

namespace ramen
{
namespace dependency
{

void graph_t::clear_sub_graph_dirty( sub_graph_t *g)
{
    BOOST_FOREACH( dependency::node_t *n, g->nodes())
        n->clear_dirty();
}

void graph_t::notify_sub_graph( sub_graph_t *g)
{
    BOOST_FOREACH( dependency::node_t *n, g->nodes())
    {
        if( n->dirty())
        {
            n->notify();
            n->clear_dirty();
        }
    }
}

graph_t::graph_t( nodes::world_node_t *w)
{
    RAMEN_ASSERT( w);

    world_ = w;
}

void graph_t::clear_all_dirty()
{
    for_each_sub_graph( &graph_t::clear_sub_graph_dirty);
}

void graph_t::notify_all_dirty()
{
    for_each_sub_graph( &graph_t::notify_sub_graph);
}

void graph_t::for_each_sub_graph( const boost::function<void( sub_graph_t*)>& f)
{
    f( world_);

    BOOST_FOREACH( nodes::node_t& n, world_->graph().nodes())
        n.for_each_sub_graph( &f);
}

} // dependency
} // ramen
