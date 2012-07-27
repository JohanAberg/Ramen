// Copyright (c) 2012 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/dependency/dependency.hpp>

#include<ramen/assert.hpp>

namespace ramen
{
namespace dependency
{

dependency_t::dependency_t( node_t *src, node_t *dst)
{
    RAMEN_ASSERT( src);
    RAMEN_ASSERT( dst);

    src_ = src;
    dst_ = dst;
}

} // namespace
} // namespace
