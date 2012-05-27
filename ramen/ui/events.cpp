// Copyright (c) 2010 Esteban Tovagliari

#include<ramen/ui/events.hpp>

namespace ramen
{
namespace ui
{

event_t::event_t() : modifiers( no_modifier)
{
    aspect_ratio = 1.0f;
    pixel_scale = 1.0f;
    subsample = 1;
}

void event_t::update_viewer_overlay() {}

key_event_t::key_event_t() : key( no_key) {}

mouse_event_t::mouse_event_t() : button( no_button) {}

paint_event_t::paint_event_t()
{
    aspect_ratio = 1.0f;
    pixel_scale = 1.0f;
    subsample = 1;
}

} // namespace
} // namespace
