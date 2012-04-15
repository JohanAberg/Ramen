// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/image/generic/composite.hpp>

#include<ramen/gil/extension/algorithm/tbb/tbb_transform.hpp>

#include<ramen/image/generic/layer_modes.hpp>

namespace ramen
{
namespace image
{
namespace generic
{

void composite_screen( const const_image_view_t& back, const const_image_view_t& front, const image_view_t& dst, float opacity)
{
    composite( back, front, dst, screen_layer_mode_fun( opacity));
}

void composite_overlay( const const_image_view_t& back, const const_image_view_t& front, const image_view_t& dst, float opacity)
{
    composite( back, front, dst, overlay_layer_mode_fun( opacity));
}

void composite_zero_overlay( const const_image_view_t& back, const image_view_t& dst, float opacity)
{
    boost::gil::tbb_transform_pixels( back, dst, zero_overlay_layer_mode_fun( opacity));
}

void composite_diff( const const_image_view_t& back, const const_image_view_t& front, const image_view_t& dst, float opacity)
{
    composite( back, front, dst, diff_layer_mode_fun( opacity));
}

} // namespace
} // namespace
} // namespace
