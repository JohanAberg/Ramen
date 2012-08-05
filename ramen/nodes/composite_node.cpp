// Copyright (c) 2012 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/nodes/composite_node.hpp>

#include<boost/bind.hpp>
#include<boost/foreach.hpp>
#include<boost/range/algorithm/for_each.hpp>

#include<ramen/assert.hpp>

#include<ramen/nodes/world_node.hpp>
#include<ramen/nodes/factory.hpp>

#include<ramen/util/string.hpp>

#include<ramen/ui/graph_layout.hpp>

namespace ramen
{
namespace nodes
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
    boost::range::for_each( graph().nodes(), boost::bind( &node_t::cloned, _1));
}

void composite_node_t::accept( visitor_t& v) { v.visit( this);}

node_t *composite_node_t::create_node_by_id( const std::string& id, bool ui)
{
    std::auto_ptr<node_t> n( do_create_node_by_id( id, ui));
    node_t *nn = n.get();

    if( nn)
    {
        add_node( n);
        nn->init();
    }

    return nn;
}

std::auto_ptr<node_t> composite_node_t::do_create_node_by_id( const std::string& id, bool ui) const
{
    return factory_t::instance().create_by_id( id, ui);
}

node_t *composite_node_t::create_node_by_id_with_version( const std::string& id, const std::pair<int, int>& version)
{
    std::auto_ptr<node_t> n( do_create_node_by_id_with_version( id, version));
    node_t *nn = n.get();

    if( nn)
    {
        add_node( n);
        nn->init();
    }

    return nn;
}

std::auto_ptr<node_t> composite_node_t::do_create_node_by_id_with_version( const std::string& id, const std::pair<int, int>& version) const
{
    return factory_t::instance().create_by_id_with_version( id, version);
}

std::auto_ptr<node_t> composite_node_t::create_unknown_node( const std::string& id, const std::pair<int, int>& version) const
{
    // TODO: implent this.
    return std::auto_ptr<node_t>();
    RAMEN_ASSERT( false);
}

void composite_node_t::add_node( std::auto_ptr<node_t> n)
{
    RAMEN_ASSERT( n.get());

    world_node_t *w = world();
    RAMEN_ASSERT( w);

    // rename the node, if needed
    std::string new_name( n->name());
    RAMEN_ASSERT( util::is_string_valid_identifier( new_name));

    while( true)
    {
        if( w->find_node( new_name))
            util::increment_string_number( new_name);
        else
            break;
    }

    n->set_name( new_name);
    n->set_parent( this);

    // save a pointer to the node.
    node_t *nn = n.get();
    g_.add_node( n);
    w->node_added( nn);
}

std::auto_ptr<node_t> composite_node_t::release_node( node_t *n)
{
    RAMEN_ASSERT( n);

    world_node_t *w = world();
    RAMEN_ASSERT( w);

    n->set_parent( 0);
    w->node_released( n);
    return g_.release_node( n);
}

const node_t *composite_node_t::find_node( const std::string& name) const
{
    composite_node_t& self = const_cast<composite_node_t&>( *this);
    return self.find_node( name);
}

node_t *composite_node_t::find_node( const std::string& name)
{
    if( name.empty())
        return 0;

    if( name[0] == '/')
    {
        if( world_node_t *w = world())
            return world()->find_node( name);
    }

    boost::char_separator<char> sep("/");
    name_tokenizer_type tok( name, sep);

    name_tokenizer_type::iterator tok_it( tok.begin());
    name_tokenizer_type::iterator tok_end( tok.end());
    return do_find_node( tok_it, tok_end);
}

node_t *composite_node_t::do_find_node( name_tokenizer_type::iterator tok_it, name_tokenizer_type::iterator tok_end)
{
    RAMEN_ASSERT( tok_it != tok_end);

    BOOST_FOREACH( node_t& n, graph().nodes())
    {
        if( n.name() == *tok_it)
        {
            ++tok_it;

            if( tok_it == tok_end)
                return &n;

            if( composite_node_t *cn = dynamic_cast<composite_node_t*>( &n))
            {
                node_t *nn = cn->do_find_node( tok_it, tok_end);

                if( nn)
                    return nn;
            }
        }
    }

    return 0;
}

/*
void composite_node_t::connect( node_t *src, const base::name_t& src_port, const base::name_t& dst_port)
{
    RAMEN_ASSERT( src->parent_node() );
    RAMEN_ASSERT( dst->parent_node() );
    RAMEN_ASSERT( src->parent_node() == dst->parent_node() );

    return dst->accept_connection( src, src_port, dst_port );
}
*/

void composite_node_t::set_layout( std::auto_ptr<ui::graph_layout_t> layout) { layout_ = layout;}

void composite_node_t::for_each_sub_graph( const boost::function<void( sub_graph_t*)> *f)
{
    sub_graph_t::for_each_sub_graph( f);

    BOOST_FOREACH( nodes::node_t& n, graph().nodes())
        n.for_each_sub_graph( f);
}

} // namespace
} // namespace
