// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/params/separator_param.hpp>

#include<ramen/app/application.hpp>

namespace ramen
{
namespace params
{

separator_param_t::separator_param_t() : param_t() { set_static( true);}

separator_param_t::separator_param_t( const std::string& name) : param_t( name) { set_static( true);}

separator_param_t::separator_param_t( const separator_param_t& other) : param_t( other) {}

} // namespace
} // namespace
