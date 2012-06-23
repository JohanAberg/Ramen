// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/params/se_expr_param.hpp>

#include<boost/bind.hpp>

#include<ramen/nodes/node.hpp>

#include<ramen/app/application.hpp>

namespace ramen
{
namespace params
{

se_expr_param_t::se_expr_param_t( const std::string& name, const create_expr_fun_type& create_expr_fun) : static_param_t( name)
{
    RAMEN_ASSERT( create_expr_fun);

    create_expr_fun_ = create_expr_fun;
    set_default_value( std::string());
}

se_expr_param_t::se_expr_param_t( const se_expr_param_t& other) : static_param_t( other)
{
    create_expr_fun_ = other.create_expr_fun_;
}

void se_expr_param_t::set_default_value( const std::string& x) { value().assign( x);}

void se_expr_param_t::set_value( const std::string& x, change_reason reason)
{
    if( can_undo())
        param_set()->add_command( this);

    value().assign( x);
    emit_param_changed( reason);
}

void se_expr_param_t::do_add_to_hash( hash::generator_t& hash_gen) const
{
    hash_gen << get_value<std::string>( *this);
}

boost::python::object se_expr_param_t::to_python( const poly_param_value_t& v) const
{
    return boost::python::object( v.cast<std::string>());
}

poly_param_value_t se_expr_param_t::from_python( const boost::python::object& obj) const
{
    std::string str = boost::python::extract<std::string>( obj);
    return poly_param_value_t( str);
}

void se_expr_param_t::do_read( const serialization::yaml_node_t& node)
{
    std::string val;
    node.get_value<std::string>( "value", val);
    set_value( val);
}

void se_expr_param_t::do_write( serialization::yaml_oarchive_t& out) const
{
    out << YAML::Key << "value"
        << YAML::Value << get_value<std::string>( *this);
}

} // namespace
} // namespace
