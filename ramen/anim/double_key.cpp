// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/anim/double_key.hpp>

#include<OpenEXR/ImathFun.h>

#include<ramen/serialization/yaml_node.hpp>
#include<ramen/serialization/yaml_oarchive.hpp>

namespace ramen
{
namespace anim
{

double_key_t::double_key_t() : keyframe_t(), v0_( 0), v1_( 0), value_( 0)
{
    auto_v0_ = auto_v1_ = tangent_smooth;
    tangent_cont_ = true;
}

double_key_t::double_key_t( time_type time, value_type value) : keyframe_t( time), value_( value), v0_( 0), v1_( 0)
{
    auto_v0_ = auto_v1_ = tangent_smooth;
    tangent_cont_ = true;
}

void double_key_t::swap( double_key_t& other)
{
    keyframe_t::swap( other);
    std::swap( value_, other.value_);
    std::swap( v0_, other.v0_);
    std::swap( v1_, other.v1_);
    std::swap( tangent_cont_, other.tangent_cont_);

    for( int i = 0; i < 4; ++i)
        std::swap( coeffs_[i], other.coeffs_[i]);
}

void double_key_t::swap_value( double_key_t& other)
{
    keyframe_t::swap_value( other);
    std::swap( value_, other.value_);
    std::swap( v0_, other.v0_);
    std::swap( v1_, other.v1_);
    std::swap( tangent_cont_, other.tangent_cont_);

    for( int i = 0; i < 4; ++i)
        std::swap( coeffs_[i], other.coeffs_[i]);
}

void double_key_t::set_v0_auto_tangent( auto_tangent_method m)
{
    auto_v0_ = m;

    if( m == tangent_smooth || m == tangent_flat)
        set_tangent_continuity( true);
    else
    {
        if( m != tangent_fixed)
            set_tangent_continuity( false);
    }
}

void double_key_t::set_v1_auto_tangent( double_key_t::auto_tangent_method m)
{
    auto_v1_ = m;

    if( m == tangent_smooth || m == tangent_flat)
        set_tangent_continuity( true);
    else
    {
        if( m != tangent_fixed)
            set_tangent_continuity( false);
    }
}

void double_key_t::set_v0_tangent( value_type slope)
{
    v0_ = Imath::clamp( slope, min_slope(), max_slope());
    set_v0_auto_tangent( tangent_fixed);

    if( tangent_cont_)
    {
        v1_ = Imath::clamp( slope, min_slope(), max_slope());
        set_v1_auto_tangent( tangent_fixed);
    }
}

void double_key_t::set_v1_tangent( value_type slope)
{
    v1_ = Imath::clamp( slope, min_slope(), max_slope());
    set_v1_auto_tangent( tangent_fixed);

    if( tangent_cont_)
    {
        v0_ = Imath::clamp( slope, min_slope(), max_slope());
        set_v0_auto_tangent( tangent_fixed);
    }
}

void double_key_t::calc_tangents( const double_key_t *prev, const double_key_t *next)
{
    if( v0_auto_tangent() == tangent_fixed && v1_auto_tangent() == tangent_fixed)
        return;

    // left
    if( !prev || ( v0_auto_tangent() == tangent_step) || ( v0_auto_tangent() == tangent_flat))
        set_v0( 0);
    else
    {
        if( v0_auto_tangent() == tangent_linear)
            set_v0( ( value() - prev->value()) / ( time() - prev->time()));
        else
        {
            if( v0_auto_tangent() == tangent_smooth)
            {
                if( !next)
                    set_v0( 0);
                else
                    set_v0( ( next->value() - prev->value()) / ( next->time() - prev->time()));
            }
        }
    }

    // right
    if( !next || ( v1_auto_tangent() == tangent_step) || ( v1_auto_tangent() == tangent_flat))
        set_v1( 0);
    else
    {
        if( v1_auto_tangent() == tangent_linear)
            set_v1( ( next->value() - value()) / ( next->time() - time()));
        else
        {
            if( v1_auto_tangent() == tangent_smooth)
            {
                if( !prev)
                    set_v1( 0);
                else
                    set_v1( ( next->value() - prev->value()) / ( next->time() - prev->time()));
            }
        }
    }
}

void double_key_t::calc_cubic_coefficients( const double_key_t& next)
{
    if( v1_auto_tangent() == tangent_step)
    {
        coeffs_[0] = 0;
        coeffs_[1] = 0;
        coeffs_[2] = 0;
        coeffs_[3] = value();
    }
    else
    {
        float time_span = next.time() - time();
        coeffs_[0] =  ( 2 * value()) - ( 2 * next.value()) + (     time_span * v1()) + ( time_span * next.v0());
        coeffs_[1] = -( 3 * value()) + ( 3 * next.value()) - ( 2 * time_span * v1()) - ( time_span * next.v0());
        coeffs_[2] = time_span * v1();
        coeffs_[3] = value();
    }
}

double_key_t::value_type double_key_t::evaluate_cubic( time_type t) const
{
    return (((( coeffs_[0] * t) + coeffs_[1]) * t + coeffs_[2]) * t) + coeffs_[3];
}

double_key_t::value_type double_key_t::evaluate_derivative( time_type t) const
{
    return (( 3.0 * coeffs_[0] * t) + ( 2.0 * coeffs_[1]) * t) + coeffs_[2];
}

double_key_t::value_type double_key_t::max_slope() { return 21.0;}
double_key_t::value_type double_key_t::min_slope() { return -max_slope();}

void double_key_t::str( std::stringstream& s) const
{
    s << time() << "," << value() << "," << v0() << "," << v1()
        << "," << v0_auto_tangent() << "," << v1_auto_tangent();
}

} // namespace
} // namespace
