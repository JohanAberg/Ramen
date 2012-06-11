// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_COMPVIEW_DRAW_PICK_GENERIC_HPP
#define RAMEN_COMPVIEW_DRAW_PICK_GENERIC_HPP

#include<OpenEXR/ImathBox.h>

#include<ramen/ui/compview/pick_result.hpp>

class QPainter;

namespace ramen
{
namespace ui
{

class composition_view_t;

int generic_node_width( const nodes::node_t *n);
int generic_node_height();

Imath::V2f generic_input_location( const nodes::node_t *n, std::size_t i);
Imath::V2f generic_output_location( const nodes::node_t *n);

void draw_generic_node( QPainter& painter, const nodes::node_t *n);

void pick_generic_node( nodes::node_t *n, const Imath::V2f& p, const composition_view_t& view, pick_result_t& result);
bool box_pick_generic_node( const nodes::node_t *n, const Imath::Box2f& b);

} // ui
} // ramen

#endif
