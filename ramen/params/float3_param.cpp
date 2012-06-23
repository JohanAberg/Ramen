// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/params/float3_param.hpp>

#include<limits>

#include<boost/bind.hpp>

#include<OpenEXR/ImathFun.h>

#include<ramen/nodes/node.hpp>

#include<ramen/app/application.hpp>
#include<ramen/app/document.hpp>

#include<ramen/params/parameterised.hpp>

#include<ramen/python/util.hpp>

namespace ramen
{
namespace params
{

float3_param_t::float3_param_t( const std::string& name) : proportional_param_t( name) { private_init();}

float3_param_t::float3_param_t( const float3_param_t& other) : proportional_param_t( other)
{
}

void float3_param_t::private_init()
{
    add_expression( name_t( "X"));
    add_expression( name_t( "Y"));
    add_expression( name_t( "Z"));

    add_curve( name_t( "X"));
    add_curve( name_t( "Y"));
    add_curve( name_t( "Z"));
    set_default_value( Imath::V3f( 0, 0, 0));
}

poly_regular_t float3_param_t::value_at_frame( float frame) const
{
    Imath::V3f v( get_value<Imath::V3f>( *this));

    if( !eval_expression( 0, frame, v.x))
        eval_curve( 0, frame, v.x);

    if( !eval_expression( 1, frame, v.y))
        eval_curve( 1, frame, v.y);

    if( !eval_expression( 2, frame, v.z))
        eval_curve( 2, frame, v.z);

    poly_indexable_regular_t val( v);
    return poly_cast<poly_regular_t&>( val);
}

void float3_param_t::set_default_value( const Imath::V3f& x)
{
    poly_indexable_regular_t v( x);
    value() = poly_cast<poly_regular_t&>( v);
}

void float3_param_t::set_value( const Imath::V3f& x, change_reason reason)
{
    float frame = 1.0f;

    //if( composition_t * c = composition())
    //    frame = c->frame();

    set_value_at_frame( x, frame, reason);
}

void float3_param_t::set_value_at_frame( const Imath::V3f& x, float frame, change_reason reason)
{
    //RAMEN_ASSERT( expression( 0).empty());
    //RAMEN_ASSERT( expression( 1).empty());
    //RAMEN_ASSERT( expression( 2).empty());

    if( can_undo())
        param_set()->add_command( this);

    poly_indexable_regular_t v( x);
    value() = poly_cast<poly_regular_t&>( v);

    bool autokey = param_set()->autokey();

    if( !is_static() && ( autokey || !curve( 0).empty()))
        curve( 0).insert( frame, x.x);

    if( !is_static() && ( autokey || !curve( 1).empty()))
        curve( 1).insert( frame, x.y);

    if( !is_static() && ( autokey || !curve( 2).empty()))
        curve( 2).insert( frame, x.z);

    evaluate( frame);
    emit_param_changed( reason);
}

Imath::V3f float3_param_t::derive( float time) const
{
    Imath::V3f result( 0.0f, 0.0f, 0.0f);

    if( !curve( 0).empty())
        result.x = curve( 0).derive( time);

    if( !curve( 1).empty())
        result.y = curve( 1).derive( time);

    if( !curve( 2).empty())
        result.z = curve( 2).derive( time);

    return result;
}

Imath::V3f float3_param_t::integrate( float time1, float time2) const
{
    Imath::V3f result = get_value<Imath::V3f>( *this);
    result.x = (time2 - time1) * result.x;
    result.y = (time2 - time1) * result.y;
    result.z = (time2 - time1) * result.z;

    if( !curve( 0).empty())
        result.x = curve( 0).integrate( time1, time2);

    if( !curve( 1).empty())
        result.y = curve( 1).integrate( time1, time2);

    if( !curve( 2).empty())
        result.z = curve( 2).integrate( time1, time2);

    return result;
}

void float3_param_t::do_add_to_hash( hash::generator_t& hash_gen) const
{
    Imath::V3f v( get_value<Imath::V3f>( *this));
    hash_gen << v.x << "," << v.y << "," << v.z;
}

boost::python::object float3_param_t::to_python( const poly_regular_t& v) const
{
    return python::vec_to_list( v.cast<Imath::V3f>());
}

poly_regular_t float3_param_t::from_python( const boost::python::object& obj) const
{
    boost::python::list t = boost::python::extract<boost::python::list>( obj);
    Imath::V3f val = python::list_to_vec3<float>( t);
    poly_indexable_regular_t v( val);
    return poly_cast<poly_regular_t&>( v);
}

} // namespace
} // namespace
