// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/params/button_param.hpp>

namespace ramen
{
namespace params
{

button_param_t::button_param_t( const std::string& name) : param_t( name) { set_static( true);}
button_param_t::button_param_t( const button_param_t& other) : param_t( other)
{
}

} // namespace
} // namespace
