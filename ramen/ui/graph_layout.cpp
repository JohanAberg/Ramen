// Copyright (c) 2012 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/graph_layout.hpp>

namespace ramen
{
namespace ui
{

graph_layout_t::graph_layout_t() {}
graph_layout_t::graph_layout_t( const graph_layout_t& other) {}

graph_layout_t::~graph_layout_t() {}

graph_layout_t *graph_layout_t::clone() const { return do_clone();}

/*************************************************************************/

simple_graph_layout_t::simple_graph_layout_t() : graph_layout_t() {}

simple_graph_layout_t::simple_graph_layout_t( const simple_graph_layout_t& other) : graph_layout_t( other) {}

graph_layout_t *simple_graph_layout_t::do_clone() const
{
    return new simple_graph_layout_t( *this);
}

} // namespace
} // namespace
