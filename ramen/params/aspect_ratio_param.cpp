// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/params/aspect_ratio_param.hpp>

#include<ramen/params/param_set.hpp>

namespace ramen
{
namespace params
{

aspect_ratio_param_t::presets_type aspect_ratio_param_t::presets_;

aspect_ratio_param_t::aspect_ratio_param_t( const std::string& name) : static_param_t( name)
{
    set_default_value( 1.0f);
}

aspect_ratio_param_t::aspect_ratio_param_t( const aspect_ratio_param_t& other) : static_param_t( other)
{
}

void aspect_ratio_param_t::set_default_value( float x) { value().assign( x);}

void aspect_ratio_param_t::set_value( float x, change_reason reason)
{
    if( param_set() && can_undo())
        param_set()->add_command( this);

    value().assign( x);
    emit_param_changed( reason);
}

void aspect_ratio_param_t::do_add_to_hash( hash::generator_t& hash_gen) const { hash_gen << get_value<float>( *this);}

boost::python::object aspect_ratio_param_t::to_python( const base::poly_regular_t& v) const
{
    return boost::python::object( get_value<float>( *this));
}

base::poly_regular_t aspect_ratio_param_t::from_python( const boost::python::object& obj) const
{
    float val = boost::python::extract<float>( obj);
    return base::poly_regular_t( val);
}

void aspect_ratio_param_t::init_presets()
{
    static bool inited = false;

    if( !inited)
    {
        presets_.push_back( std::make_pair( "Square", 1.0f));
        presets_.push_back( std::make_pair( "PAL 1.067", 1.067f));
        presets_.push_back( std::make_pair( "NTSC 0.91", 0.91f));
        presets_.push_back( std::make_pair( "HD 1440 1.33", 1.33f));
        presets_.push_back( std::make_pair( "PAL 16:9 1.46", 1.46f));
        presets_.push_back( std::make_pair( "NTSC 16:9 1.21", 1.21f));
        presets_.push_back( std::make_pair( "Cinemascope 2", 2.0f));
        inited = true;
    }
}

const aspect_ratio_param_t::presets_type& aspect_ratio_param_t::presets()
{
    init_presets();
    return presets_;
}

void aspect_ratio_param_t::add_preset( const std::pair<std::string, float>& p)
{
    for( int i = 0; i < presets_.size(); ++i)
    {
        if( p == presets_[i])
            return;
    }

    presets_.push_back( p);
}

int aspect_ratio_param_t::index_for_value( float x) const
{
    for( int i = 0; i < presets().size(); ++i)
    {
        if( presets()[i].second == x)
            return i;
    }

    return presets().size();
}

} // namespace
} // namespace
