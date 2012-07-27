// Copyright (c) 2012 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/dependency/node.hpp>

namespace ramen
{
namespace dependency
{

node_t::node_t() : dirty_( false) {}
node_t::node_t( const node_t& other) : dirty_( false) {}

node_t::~node_t() {}

bool node_t::dirty() const { return dirty_;}

void node_t::set_dirty()    { dirty_ = true;}
void node_t::clear_dirty()	{ dirty_ = false;}

void node_t::notify()       { do_notify();}
void node_t::do_notify()    {}

/**************************************************/

output_node_t::output_node_t() : node_t() {}
output_node_t::output_node_t( const output_node_t& other) : node_t( other) {}

void output_node_t::do_notify() { changed();}

} // namespace
} // namespace
