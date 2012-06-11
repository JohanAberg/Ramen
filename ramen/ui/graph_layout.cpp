// Copyright (c) 2012 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/python/python.hpp>

#include<ramen/ui/graph_layout.hpp>

#include<ramen/assert.hpp>

#include<ramen/nodes/node.hpp>
#include<ramen/nodes/visitor.hpp>

namespace ramen
{
namespace ui
{
namespace
{

struct bbox_node_visitor : public nodes::generic_visitor_t
{
    bbox_node_visitor() {}

    virtual void visit( nodes::node_t *n)
    {
        // TODO: implement this.
        RAMEN_ASSERT( false);
    }

    Imath::Box2f bbox;
};

} // unnamed

graph_layout_t::graph_layout_t() {}
graph_layout_t::graph_layout_t( const graph_layout_t& other) {}

graph_layout_t::~graph_layout_t() {}

graph_layout_t *graph_layout_t::clone() const { return do_clone();}

Imath::Box2f graph_layout_t::node_bbox( const nodes::node_t& n) const
{
    nodes::node_t& nn = const_cast<nodes::node_t&>( n);

    bbox_node_visitor v;
    nn.accept( v);
    return v.bbox;
}

/*************************************************************************/

simple_graph_layout_t::simple_graph_layout_t() : graph_layout_t() {}

simple_graph_layout_t::simple_graph_layout_t( const simple_graph_layout_t& other) : graph_layout_t( other) {}

graph_layout_t *simple_graph_layout_t::do_clone() const
{
    return new simple_graph_layout_t( *this);
}

void simple_graph_layout_t::place_node( nodes::node_t& n, const nodes::graph_t& g)
{
    // TODO: implement this.
    RAMEN_ASSERT( false);
}

void simple_graph_layout_t::layout_selected_nodes( nodes::graph_t& g)
{
    // TODO: implement this.
    RAMEN_ASSERT( false);
}

} // namespace
} // namespace
