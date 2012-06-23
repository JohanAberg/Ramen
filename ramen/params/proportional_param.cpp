// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/params/proportional_param.hpp>

namespace ramen
{
namespace params
{

Imath::V3f proportional_param_t::proportional_factor = Imath::V3f( 0, 0, 0);

proportional_param_t::proportional_param_t( const std::string& name) : numeric_param_t( name) {}

proportional_param_t::proportional_param_t( const proportional_param_t& other) : numeric_param_t( other)
{
}

void proportional_param_t::set_proportional( bool p)
{
    if( p)
        set_flags( flags() | proportional_bit);
    else
        set_flags( flags() & ~proportional_bit);
}

} // namespace
} // namespace
