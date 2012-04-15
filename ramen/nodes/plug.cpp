// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/nodes/plug.hpp>

#include<ramen/assert.hpp>

namespace ramen
{

output_plug_t::output_plug_t() {}

output_plug_t::output_plug_t( const output_plug_t& other) {}

void output_plug_t::add_output( node_t *n, int port)
{
    RAMEN_ASSERT( std::find( nodes_.begin(), nodes_.end(), std::make_pair( n, port)) == nodes_.end());
    nodes_.push_back( std::make_pair( n, port));
}

void output_plug_t::remove_output( node_t *n, int port)
{
    std::vector<std::pair<node_t*, int> >::iterator it( std::find( nodes_.begin(), nodes_.end(), std::make_pair( n, port)));

    if( it != nodes_.end())
        nodes_.erase( it);
}

const output_plug_t::value_type& output_plug_t::operator[]( int ix) const
{
    RAMEN_ASSERT( ix < nodes_.size());
    return nodes_[ix];
}

} // namespace
