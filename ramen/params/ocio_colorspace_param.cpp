// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/params/ocio_colorspace_param.hpp>

#include<ramen/params/param_set.hpp>
#include<ramen/nodes/node.hpp>

#include<ramen/app/application.hpp>

#include<ramen/ocio/manager.hpp>

namespace ramen
{
namespace params
{

ocio_colorspace_param_t::ocio_colorspace_param_t( const std::string& name) : static_param_t( name)
{
    set_default_value( default_colorspace());
}

ocio_colorspace_param_t::ocio_colorspace_param_t( const ocio_colorspace_param_t& other) : static_param_t( other) {}

void ocio_colorspace_param_t::set_default_value( const std::string& cs) { value().assign( cs);}

void ocio_colorspace_param_t::set_value( const std::string& cs, change_reason reason)
{
    if( param_set() && can_undo())
        param_set()->add_command( this);

    value().assign( cs);
    emit_param_changed( reason);
}

void ocio_colorspace_param_t::do_add_to_hash( hash::generator_t& hash_gen) const
{
    hash_gen << get_value<std::string>( *this);
}

boost::python::object ocio_colorspace_param_t::to_python( const base::poly_regular_t& v) const
{
    return boost::python::object( v.cast<std::string>());
}

base::poly_regular_t ocio_colorspace_param_t::from_python( const boost::python::object& obj) const
{
    std::string str = boost::python::extract<std::string>( obj);
    return base::poly_regular_t( str);
}

void ocio_colorspace_param_t::do_read( const serialization::yaml_node_t& node)
{
    serialization::yaml_node_t n = node.get_node( "value");
    std::string val;
    n >> val;

    OCIO::ConstConfigRcPtr config = app().ocio_manager().config();
    int index = -1;

    int num_color_spaces = config->getNumColorSpaces();

    for(int i = 0; i < num_color_spaces; i++)
    {
        std::string csname = config->getColorSpaceNameByIndex( i);

        if( csname == val)
            index = i;
    }

    if( index != -1)
        value().assign( val);
    else
    {
        node.error_stream() << "Node " << parameterised()->name() << ": colorspace " << val << " not found in OCIO config.\n";
        node.error_stream() << "Replacing by default value.\n";
        value().assign( default_colorspace());
    }
}

void ocio_colorspace_param_t::do_write( serialization::yaml_oarchive_t& out) const
{
    out << YAML::Key << "value" << YAML::Value << get_value<std::string>( *this);
}

std::string ocio_colorspace_param_t::default_colorspace() const
{
    OCIO::ConstConfigRcPtr config = app().ocio_manager().config();
    return config->getColorSpace( OCIO::ROLE_SCENE_LINEAR)->getName();
}

} // namespace
} // namespace
