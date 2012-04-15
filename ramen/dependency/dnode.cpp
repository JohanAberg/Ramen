// Copyright (c) 2012 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/dependency/dnode.hpp>

namespace ramen
{
namespace dependency
{

dnode_t::dnode_t() : dirty_( false) {}
dnode_t::dnode_t( const dnode_t& other) : dirty_( false) {}

dnode_t::~dnode_t() {}

bool dnode_t::dirty() const			{ return dirty_;}
void dnode_t::set_dirty( bool d)	{ dirty_ = d;}

void dnode_t::notify() {}

/**************************************************/

output_dnode_t::output_dnode_t() : dnode_t() {}
output_dnode_t::output_dnode_t( const output_dnode_t& other) : dnode_t( other) {}

void output_dnode_t::notify() { changed();}

} // namespace
} // namespace
