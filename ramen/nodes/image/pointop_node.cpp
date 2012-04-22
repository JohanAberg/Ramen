// Copyright (c) 2010 Esteban Tovagliari

#include<ramen/nodes/image/pointop_node.hpp>

namespace ramen
{
namespace image
{

pointop_node_t::pointop_node_t() : image_node_t()
{
    add_input_plug( input_plug_info_t( ui::palette_t::instance().color("front plug"), "Front"));
    add_output_plug();
}

pointop_node_t::pointop_node_t( const pointop_node_t& other) : image_node_t( other) {}

void pointop_node_t::do_process( const render::context_t& context)
{
    Imath::Box2i area( Imath::intersect( input_as<image_node_t>()->defined(), defined()));

    if( area.isEmpty())
		return;

    do_process( input_as<image_node_t>()->const_subimage_view( area), subimage_view( area), context);
}

} // namespace
} // namespace
