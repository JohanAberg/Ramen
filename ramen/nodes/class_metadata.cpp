// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/python/python.hpp>

#include<ramen/nodes/class_metadata.hpp>

#include<ramen/nodes/node.hpp>

namespace ramen
{
namespace nodes
{

class_metadata_t::class_metadata_t()
{
    ui_visible = true;
    help = 0;
    first_time_ = true;
}

} // namespace
} // namespace
