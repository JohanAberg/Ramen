// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/params/bool_param.hpp>

#include<ramen/app/application.hpp>

#include<ramen/nodes/node.hpp>

namespace ramen
{
namespace params
{

bool_param_t::bool_param_t( const std::string& name) : static_param_t( name)
{
    set_default_value( false);
}

bool_param_t::bool_param_t( const bool_param_t& other) : static_param_t( other)
{
}

void bool_param_t::set_default_value( bool x) { value().assign( x);}

void bool_param_t::set_value( bool x, change_reason reason)
{
    if( can_undo())
        param_set()->add_command( this);

    value().assign( x);
    emit_param_changed( reason);
}

param_t *bool_param_t::do_clone() const { return new bool_param_t( *this);}

void bool_param_t::do_add_to_hash( hash::generator_t& hash_gen) const { hash_gen << get_value<bool>( *this);}

boost::python::object bool_param_t::to_python( const base::poly_regular_t& v) const
{
    return boost::python::object( v.cast<bool>());
}

base::poly_regular_t bool_param_t::from_python( const boost::python::object& obj) const
{
    bool b = boost::python::extract<bool>( obj);
    return base::poly_regular_t( b);
}

void bool_param_t::do_read( const serialization::yaml_node_t& node)
{
    bool val;
    node.get_value<bool>( "value", val);
    set_value( val, silent_edit);
}

void bool_param_t::do_write( serialization::yaml_oarchive_t& out) const
{
    out << YAML::Key << "value"
        << YAML::Value << get_value<bool>( *this);
}

} // namespace
} // namespace
