// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/params/ocio_colorspace_param.hpp>

#include<ramen/app/application.hpp>

#include<ramen/ocio/manager.hpp>

#include<ramen/params/param_set.hpp>
#include<ramen/nodes/node.hpp>

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

std::string ocio_colorspace_param_t::default_colorspace() const
{
    OCIO::ConstConfigRcPtr config = app().ocio_manager().config();
    return config->getColorSpace( OCIO::ROLE_SCENE_LINEAR)->getName();
}

} // namespace
} // namespace
