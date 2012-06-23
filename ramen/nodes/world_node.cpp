// Copyright (c) 2012 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/nodes/world_node.hpp>

#include<boost/bind.hpp>
#include<boost/range/algorithm/for_each.hpp>

#include<ramen/assert.hpp>

namespace ramen
{
namespace nodes
{

world_node_t::world_node_t() : composite_node_t() {}

world_node_t::~world_node_t()
{
    boost::range::for_each( graph().nodes(), boost::bind( &world_node_t::emit_node_release_signal, this, _1));
}

void world_node_t::emit_node_release_signal( node_t& n)
{
    if( composite_node_t *cn = dynamic_cast<composite_node_t*>( &n))
        boost::range::for_each( cn->graph().nodes(), boost::bind( &world_node_t::emit_node_release_signal, this, _1));

    node_released( &n);
}

} // namespace
} // namespace
