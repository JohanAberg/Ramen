// Copyright (c) 2012 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/params/composite_parameterised.hpp>

namespace ramen
{

composite_parameterised_t::composite_parameterised_t() : parameterised_t() {}

composite_parameterised_t::composite_parameterised_t( const composite_parameterised_t& other) : parameterised_t( other)
{
}

composite_parameterised_t::~composite_parameterised_t()
{
	// empty destructor to allow auto_prt to use an incomplete type.
	// Do not remove.
}

} // namespace
