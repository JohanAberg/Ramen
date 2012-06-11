// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/python/python.hpp>

#include<ramen/nodes/visitor.hpp>

#include<ramen/nodes/composite_node.hpp>

namespace ramen
{
namespace nodes
{

visitor_t::~visitor_t() {}

generic_visitor_t::~generic_visitor_t() {}

void generic_visitor_t::visit( composite_node_t *n)
{
    visit( static_cast<node_t*>( n));
}

} // namespace
} // namespace
