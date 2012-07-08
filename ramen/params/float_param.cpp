// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/params/float_param.hpp>

#include<limits>

#include<boost/bind.hpp>

#include<ramen/app/application.hpp>

#include<ramen/params/parameterised.hpp>

#include<ramen/serialization/yaml_node.hpp>
#include<ramen/serialization/yaml_oarchive.hpp>

namespace ramen
{
namespace params
{

float_param_t::float_param_t( const std::string& name) : numeric_param_t( name) { private_init();}
float_param_t::float_param_t( const float_param_t& other) : numeric_param_t( other) {}

void float_param_t::private_init()
{
    add_curve( name());
    add_expression( name());
    set_default_value( 0);
}

base::poly_regular_t float_param_t::value_at_frame( float frame) const
{
    float val = get_value<float>( *this);

    if( !eval_expression( 0, frame, val))
        eval_curve( 0, frame, val);

    return base::poly_regular_t( val);
}

void float_param_t::set_default_value( float x) { value().assign( x);}

void float_param_t::set_value( float x, change_reason reason)
{
    float frame = 1.0;

    //if( composition_t * c = composition())
    //    frame = c->frame();

    set_value_at_frame( x, frame, reason);
}

void float_param_t::set_value_at_frame( float x, float frame, change_reason reason)
{
    //RAMEN_ASSERT( expression().empty());

    if( can_undo())
        param_set()->add_command( this);

    if( !is_static() && ( param_set()->autokey() || !curve().empty()))
    {
        curve().insert( frame, x);
        evaluate( frame);
    }
    else
        value().assign( x);

    emit_param_changed( reason);
}

void float_param_t::set_absolute_value( float x, change_reason reason)
{
    set_value( absolute_to_relative( round( x)), reason);
}

void float_param_t::set_absolute_value_at_frame( float x, float frame, change_reason reason)
{
    set_value_at_frame( absolute_to_relative( round( x)), frame, reason);
}

float float_param_t::derive( float time) const
{
    //RAMEN_ASSERT( expression().empty());

    if( num_curves() && !curve().empty())
        return curve().derive( time);
    else
        return 0;
}

float float_param_t::integrate( float time1, float time2) const
{
    //RAMEN_ASSERT( expression().empty());

    if( num_curves() && !curve().empty())
        return curve().integrate( time1, time2);
    else
    {
        float v = get_value<float>( *this);
        return (time2 - time1) * v;
    }
}

void float_param_t::do_add_to_hash( hash::generator_t& hash_gen) const
{
    hash_gen << get_value<float>( *this);
}

boost::python::object float_param_t::to_python( const base::poly_regular_t& v) const
{
    return boost::python::object( v.cast<float>());
}

base::poly_regular_t float_param_t::from_python( const boost::python::object& obj) const
{
    float val = boost::python::extract<float>( obj);
    return base::poly_regular_t( val);
}

} // namespace
} // namespace
