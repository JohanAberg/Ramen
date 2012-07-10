// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/params/group_param.hpp>

namespace ramen
{
namespace params
{

group_param_t::group_param_t( const std::string& name) : composite_param_t( name), can_collapse_( true) {}

group_param_t::group_param_t( const group_param_t& other) : composite_param_t( other) {}

} // namespace
} // namespace
