// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/params/image_format_param.hpp>

#include<QLabel>
#include<QComboBox>

#include<ramen/app/application.hpp>

#include<ramen/params/param_set.hpp>

#include<ramen/ui/user_interface.hpp>
#include<ramen/ui/inspector/inspector.hpp>
#include<ramen/ui/widgets/spinbox.hpp>
#include<ramen/ui/widgets/double_spinbox.hpp>

namespace ramen
{
namespace params
{

image_format_param_t::image_format_param_t( const std::string& name) : static_param_t( name) {}

image_format_param_t::image_format_param_t( const image_format_param_t& other) : static_param_t( other)
{
}

void image_format_param_t::set_value( const image::format_t& format, change_reason reason)
{
    if( can_undo())
        param_set()->add_command( this);

    value().assign( format);
    emit_param_changed( reason);
}

param_t *image_format_param_t::do_clone() const { return new image_format_param_t( *this);}

void image_format_param_t::do_init()
{
    //if( composition())
    //    value().assign( composition()->default_format());
    //else
        value().assign( image::format_t());
}

void image_format_param_t::do_add_to_hash( hash::generator_t& hash_gen) const
{
    image::format_t format( get_value<image::format_t>( *this));
    hash_gen << format.width << "," << format.height << "," << format.aspect;
}

void image_format_param_t::do_read( const serialization::yaml_node_t& node)
{
    image::format_t format;
    node.get_value<image::format_t>( "value", format);
    set_value( format, silent_edit);
}

void image_format_param_t::do_write( serialization::yaml_oarchive_t& out) const
{
    image::format_t format( get_value<image::format_t>( *this));
    out << YAML::Key << "value" << YAML::Value << format;
}

} // namespace
} // namespace
