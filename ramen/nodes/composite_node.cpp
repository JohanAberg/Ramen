// Copyright (c) 2012 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/nodes/composite_node.hpp>

#include<boost/bind.hpp>

#include<adobe/algorithm/for_each.hpp>

#include<ramen/assert.hpp>

#include<ramen/nodes/world_node.hpp>
#include<ramen/nodes/node_factory.hpp>

#include<ramen/ui/graph_layout.hpp>

namespace ramen
{

composite_node_t::composite_node_t() : node_t()
{
    layout_.reset( new ui::simple_graph_layout_t());
}

composite_node_t::composite_node_t( const composite_node_t& other) : node_t( other), g_( other.g_)
{
    layout_.reset( other.layout_->clone());
}

composite_node_t::~composite_node_t() {}

void composite_node_t::cloned()
{
    adobe::for_each( graph().nodes(), boost::bind( &node_t::cloned, _1));
}

void composite_node_t::accept( node_visitor& v) { v.visit( this);}

node_t *composite_node_t::create_node_by_id( const std::string& id, bool ui)
{
    std::auto_ptr<node_t> n( do_create_node_by_id( id, ui));
    node_t *nn = n.get();

    if( nn)
        add_node( n);

    return nn;
}

std::auto_ptr<node_t> composite_node_t::do_create_node_by_id( const std::string& id, bool ui) const
{
    return node_factory_t::instance().create_by_id( id, ui);
}

node_t *composite_node_t::create_node_by_id_with_version( const std::string& id, const std::pair<int, int>& version)
{
    std::auto_ptr<node_t> n( do_create_node_by_id_with_version( id, version));
    node_t *nn = n.get();

    if( nn)
        add_node( n);

    return nn;
}

std::auto_ptr<node_t> composite_node_t::do_create_node_by_id_with_version( const std::string& id, const std::pair<int, int>& version) const
{
    return node_factory_t::instance().create_by_id_with_version( id, version);
}

void composite_node_t::add_node( std::auto_ptr<node_t> n)
{
    RAMEN_ASSERT( false);

    /*
    RAMEN_ASSERT( n.get());

    node_t *nn = n.get();
    g_.add_node( n);
    nn->set_parent( this);
    // rename node here, if needed, more stuff, ...

    if( world_node_t *w = world())
        w->node_added( nn);
    */
}

std::auto_ptr<node_t> composite_node_t::remove_node( node_t *n)
{
    RAMEN_ASSERT( false);

    /*
    n->set_parent( 0);

    if( world_node_t *w = world())
        w->node_removed( n);

    return g_.remove_node( n);
    */
}

void composite_node_t::set_layout( std::auto_ptr<ui::graph_layout_t> layout)
{
    layout_ = layout;
}

} // namespace
