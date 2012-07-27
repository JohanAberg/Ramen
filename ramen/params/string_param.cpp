// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/params/string_param.hpp>

#include<ramen/nodes/node.hpp>

namespace ramen
{
namespace params
{

string_param_t::string_param_t( const std::string& name) : static_param_t( name)
{
    set_default_value( std::string());
    read_only_ = false;
    multiline_ = false;
}

string_param_t::string_param_t( const std::string& name, bool read_only) : static_param_t( name)
{
    set_default_value( std::string());
    multiline_ = false;
    read_only_ = read_only;

    if( read_only_)
    {
        set_can_undo( false);
        set_persist( false);
        set_include_in_hash( false);
    }
}

string_param_t::string_param_t( const string_param_t& other) : static_param_t( other)
{
    multiline_ = other.multiline_;
    read_only_ = other.read_only_;
}

void string_param_t::set_default_value( const std::string& x) { value().assign( x);}

void string_param_t::set_value( const std::string& x, change_reason reason)
{
    if( read_only_)
    {
        set_default_value( x);
        //update_widgets();
    }
    else
    {
        if( can_undo())
            param_set()->add_command( this);

        value().assign( x);
        emit_param_changed( reason);
    }
}

void string_param_t::do_add_to_hash( hash::generator_t& hash_gen) const
{
    hash_gen << get_value<std::string>( *this);
}

boost::python::object string_param_t::to_python( const base::poly_regular_t& v) const
{
    return boost::python::object( v.cast<std::string>());
}

base::poly_regular_t string_param_t::from_python( const boost::python::object& obj) const
{
    std::string str = boost::python::extract<std::string>( obj);
    return base::poly_regular_t( str);
}

} // namespace
} // namespace
