// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/anim/curve_to_half_lut.hpp>

namespace ramen
{
namespace anim
{

eval_double_curve::eval_double_curve( const double_curve_t& c) : c_( c) {}
half eval_double_curve::operator()( half x) const
{
    return c_.evaluate( static_cast<float>( x));
}

void curve_to_half_lut( const double_curve_t& c, halfFunction<half>& lut)
{
    lut = halfFunction<half>( eval_double_curve( c));
}

} // namespace
} // namespace
