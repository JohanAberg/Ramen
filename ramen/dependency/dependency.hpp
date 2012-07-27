// Copyright (c) 2012 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_DEPENDENCY_DEPENDENCY_HPP
#define RAMEN_DEPENDENCY_DEPENDENCY_HPP

#include<ramen/config.hpp>

#include<ramen/dependency/node_fwd.hpp>

namespace ramen
{
namespace dependency
{

class RAMEN_API dependency_t
{
public:

    dependency_t( node_t *src, node_t *dst);

    node_t *src() { return src_;}
    node_t *dst() { return dst_;}

private:

    node_t *src_;
    node_t *dst_;
};

} // namespace
} // namespace

#endif
