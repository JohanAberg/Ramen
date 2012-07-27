// Copyright (c) 2012 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/dependency/sub_graph.hpp>

#include<boost/foreach.hpp>
#include<boost/bind.hpp>
#include<boost/range/algorithm/for_each.hpp>

#include<ramen/dependency/node.hpp>

namespace ramen
{
namespace dependency
{

sub_graph_t::sub_graph_t() {}
sub_graph_t::~sub_graph_t() {}

void sub_graph_t::add_dependency_node( node_t *n)
{
    nodes_.push_back( n);
}

void sub_graph_t::add_dependency( node_t *src, node_t *dst)
{
    dependencies_.push_back( dependency_t( src, dst));
}

void sub_graph_t::clear_all_dirty()
{
    boost::range::for_each( nodes_, boost::bind( &node_t::clear_dirty, _1));
}

void sub_graph_t::notify_all_dirty()
{
    BOOST_FOREACH( node_t *n, nodes_)
    {
        if( n->dirty())
        {
            n->notify();
            n->clear_dirty();
        }
    }
}

void sub_graph_t::propagate_dirty_flags()
{
    BOOST_FOREACH( dependency_t& dep, dependencies_)
    {
        if( dep.src()->dirty())
            dep.dst()->set_dirty();
    }

    do_propagate_dirty_flags();
}

void sub_graph_t::do_propagate_dirty_flags() {}

void sub_graph_t::for_each_sub_graph( const boost::function<void( sub_graph_t*)> *f)
{
    (*f)( this);
}

} // dependency
} // ramen
