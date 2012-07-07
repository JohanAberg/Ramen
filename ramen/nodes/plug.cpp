// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/python/python.hpp>

#include<ramen/nodes/plug.hpp>

#include<algorithm>

#include<ramen/nodes/node.hpp>

namespace ramen
{
namespace nodes
{
namespace
{

struct match_output_connection
{
    match_output_connection( ramen::nodes::node_t *node, const base::name_t& id)
    {
        node_ = node;
        id_ = id;
    }

    bool operator()( const output_plug_t::connection_type& c) const
    {
        if( boost::get<0>( c) == node_ && boost::get<1>( c) == id_)
            return true;

        return false;
    }

    ramen::nodes::node_t *node_;
    base::name_t id_;
};

} // unnamed

input_plug_t *new_clone( const input_plug_t& other) { return new input_plug_t( other);}

output_plug_t::output_plug_t( ramen::nodes::node_t *parent, const base::name_t& id,
                                        const Imath::Color3c& color,
                                        const std::string& tooltip) : plug_t( id, color, tooltip),
                                                                    dependency::output_node_t(),
                                                                    parent_( parent)
{
    RAMEN_ASSERT( parent_);
}

output_plug_t::output_plug_t( const output_plug_t& other) : plug_t( other)
{
    parent_ = 0;
}

output_plug_t::~output_plug_t() {}

void output_plug_t::add_output( ramen::nodes::node_t *n, const base::name_t& plug)
{
    RAMEN_ASSERT( std::find_if( connections_.begin(), connections_.end(), match_output_connection( n, plug)) == connections_.end());

    int port = n->find_input( plug);
    RAMEN_ASSERT( port >= 0);

    connections_.push_back( boost::tuples::make_tuple( n, plug, port));
}

void output_plug_t::remove_output( ramen::nodes::node_t *n, const base::name_t& plug)
{
    iterator it( std::find_if( connections_.begin(), connections_.end(), match_output_connection( n, plug)));

    if( it != connections_.end())
        connections_.erase( it);
}

output_plug_t *output_plug_t::do_clone() const
{
    return new output_plug_t( *this);
}

output_plug_t *new_clone( const output_plug_t& other) { return other.clone();}

} // namespace
} // namespace
