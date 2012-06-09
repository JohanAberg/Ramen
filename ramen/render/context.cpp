// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/render/context.hpp>

namespace ramen
{
namespace render
{

context_t::context_t()
{
    mode = interface_render;
    frame = 1;
    subsample = 1;
    result_node = 0;
    proxy_level = 0;
    motion_blur_extra_samples = 0;
    motion_blur_shutter_factor = 1;
    cancel = &context_t::default_cancel;
    error_log = 0;
}

bool context_t::render_cancelled() const
{
    if( cancel)
        return cancel();

    return false;
}

bool context_t::default_cancel() { return false;}

} // namespace
} // namespace
