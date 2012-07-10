// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/params/motion_blur_param.hpp>

#include<ramen/params/param_set.hpp>

namespace ramen
{
namespace params
{

motion_blur_param_t::motion_blur_param_t( const std::string& name) : static_param_t( name)
{
    value().assign( motion_blur_info_t());
}

motion_blur_param_t::motion_blur_param_t( const motion_blur_param_t& other) : static_param_t( other)
{
}

bool motion_blur_param_t::motion_blur_enabled() const
{
    motion_blur_info_t info( get_value<motion_blur_info_t>( *this));

    if( info.samples == 1 || info.shutter == 0)
        return false;

    return true;
}

motion_blur_info_t::loop_data_t motion_blur_param_t::loop_data( float time, int extra_samples, float shutter_factor) const
{
    motion_blur_info_t info( get_value<motion_blur_info_t>( *this));
    return motion_blur_info_t::loop_data_t( time, info.samples + extra_samples, info.shutter * shutter_factor,
                                            info.shutter_offset, info.filter);
}

void motion_blur_param_t::do_add_to_hash( hash::generator_t& hash_gen) const
{
    motion_blur_info_t info( get_value<motion_blur_info_t>( *this));
    hash_gen << info.samples << "," << info.shutter << ","
            << info.shutter_offset << "," << info.filter;
}

void motion_blur_param_t::do_read( const serialization::yaml_node_t& node)
{
    motion_blur_info_t val;

    if( node.get_optional_value( "value", val))
        value().assign( val);
}

void motion_blur_param_t::do_write( serialization::yaml_oarchive_t& out) const
{
    motion_blur_info_t info( get_value<motion_blur_info_t>( *this));
    out << YAML::Key << "value" << YAML::Value << info;
}

} // namespace
} // namespace
