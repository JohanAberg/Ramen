// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/params/float2_param.hpp>

#include<limits>

#include<boost/bind.hpp>

#include<OpenEXR/ImathFun.h>

#include<QLabel>

#include<ramen/app/application.hpp>
#include<ramen/app/document.hpp>

#include<ramen/nodes/node.hpp>

#include<ramen/python/util.hpp>

#include<ramen/ui/user_interface.hpp>
#include<ramen/ui/anim/anim_editor.hpp>

#include<ramen/ui/widgets/param_spinbox.hpp>
#include<ramen/ui/inspector/inspector.hpp>

namespace ramen
{
namespace params
{

float2_param_t::float2_param_t( const std::string& name) : proportional_param_t( name) { private_init();}

float2_param_t::float2_param_t( const float2_param_t& other) : proportional_param_t( other)
{
}

void float2_param_t::private_init()
{
    add_expression( name_t( "X"));
    add_expression( name_t( "Y"));

    add_curve( name_t( "X"));
    add_curve( name_t( "Y"));

    set_default_value( Imath::V2f( 0, 0));
}

void float2_param_t::set_default_value( const Imath::V2f& x)
{
    poly_indexable_regular_t v( x);
    value() = poly_cast<poly_regular_t&>( v);
}

poly_regular_t float2_param_t::value_at_frame( float frame) const
{
    Imath::V2f v( get_value<Imath::V2f>( *this));

    if( !eval_expression( 0, frame, v.x))
        eval_curve( 0, frame, v.x);

    if( !eval_expression( 1, frame, v.y))
        eval_curve( 1, frame, v.y);

    poly_indexable_regular_t val( v);
    return poly_cast<poly_regular_t&>( val);
}

void float2_param_t::set_value( const Imath::V2f& x, change_reason reason)
{
    float frame = 1.0f;

    //if( composition_t * c = composition())
    //    frame = c->frame();

    set_value_at_frame( x, frame, reason);
}

void float2_param_t::set_value_at_frame( const Imath::V2f& x, float frame, change_reason reason)
{
    //RAMEN_ASSERT( expression( 0).empty());
    //RAMEN_ASSERT( expression( 1).empty());

    if( can_undo())
        param_set()->add_command( this);

    poly_indexable_regular_t v( x);
    value() = poly_cast<poly_regular_t&>( v);

    bool autokey = param_set()->autokey();

    if( !is_static() && ( autokey || !curve( 0).empty()))
        curve( 0).insert( frame, x.x);

    if( !is_static() && ( autokey || !curve( 1).empty()))
        curve( 1).insert( frame, x.y);

    evaluate( frame);
    emit_param_changed( reason);
}

void float2_param_t::set_absolute_value( const Imath::V2f& x, change_reason reason)
{
    set_value( absolute_to_relative( round( x)), reason);
}

void float2_param_t::set_absolute_value_at_frame( const Imath::V2f& x, float frame, change_reason reason)
{
    set_value_at_frame( absolute_to_relative( round( x)), frame, reason);
}

Imath::V2f float2_param_t::derive( float time) const
{
    Imath::V2f result( 0.0f, 0.0f);

    if( !curve( 0).empty())
        result.x = curve( 0).derive( time);

    if( !curve( 1).empty())
        result.y = curve( 1).derive( time);

    return result;
}

Imath::V2f float2_param_t::integrate( float time1, float time2) const
{
    Imath::V2f result = get_value<Imath::V2f>( *this);
    result.x  = (time2 - time1) * result.x;
    result.y  = (time2 - time1) * result.y;

    if( !curve( 0).empty())
        result.x = curve( 0).integrate( time1, time2);

    if( !curve( 1).empty())
        result.y = curve( 1).integrate( time1, time2);

    return result;
}

void float2_param_t::do_add_to_hash( hash::generator_t& hash_gen) const
{
    Imath::V2f v( get_value<Imath::V2f>( *this));
    hash_gen << v.x << "," << v.y;
}

boost::python::object float2_param_t::to_python( const poly_regular_t& v) const
{
    return python::vec_to_list( v.cast<Imath::V2f>());
}

poly_regular_t float2_param_t::from_python( const boost::python::object& obj) const
{
    boost::python::list t = boost::python::extract<boost::python::list>( obj);
    Imath::V2f val = python::list_to_vec2<float>( t);
    poly_indexable_regular_t v( val);
    return poly_cast<poly_regular_t&>( v);
}

} // namespace
} // namespace
