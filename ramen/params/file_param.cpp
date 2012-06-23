// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/params/file_param.hpp>

#include<boost/filesystem/operations.hpp>

#include<ramen/app/application.hpp>
#include<ramen/app/document.hpp>

#include<ramen/params/param_set.hpp>

namespace ramen
{
namespace params
{

file_param_t::file_param_t( const std::string& name) : static_param_t( name)
{
    is_input_ = true;
    set_default_value( boost::filesystem::path());
    ext_list_string_ = "File (*.*)";
}

file_param_t::file_param_t( const file_param_t& other) : static_param_t( other)
{
    is_input_ = other.is_input_;
    ext_list_string_ = other.ext_list_string_;
}

void file_param_t::set_default_value( const boost::filesystem::path& p) { value().assign( p);}

void file_param_t::set_value( const boost::filesystem::path& p, change_reason reason)
{
    if( can_undo())
        param_set()->add_command( this);

    value().assign( p);
    emit_param_changed( reason);
}

bool file_param_t::empty() const
{
    boost::filesystem::path p( get_value<boost::filesystem::path>( *this));
    return p.empty();
}

bool file_param_t::file_exists() const
{
    boost::filesystem::path p( get_value<boost::filesystem::path>( *this));

    if( !p.empty())
        return boost::filesystem::exists( p);

    return false;
}

std::string file_param_t::extension() const
{
    boost::filesystem::path p( get_value<boost::filesystem::path>( *this));
    return p.extension().string();
}

void file_param_t::set_extension( const std::string& ext)
{
    RAMEN_ASSERT( !is_input_);
    boost::filesystem::path p( get_value<boost::filesystem::path>( *this));
    p.replace_extension( ext);
    value().assign( p);
}

void file_param_t::do_add_to_hash( hash::generator_t& hash_gen) const
{
    if( is_input_)
        hash_gen << filesystem::hash_string( get_value<boost::filesystem::path>( *this));
}

boost::python::object file_param_t::to_python( const poly_regular_t& v) const
{
    std::string str = filesystem::file_string( v.cast<boost::filesystem::path>());
    return boost::python::object( str);
}

poly_regular_t file_param_t::from_python( const boost::python::object& obj) const
{
    std::string str = boost::python::extract<std::string>( obj);
    boost::filesystem::path p( str);
    return poly_regular_t( p);
}

void file_param_t::do_read( const serialization::yaml_node_t& node)
{
    std::string val;
    node.get_value<std::string>( "value", val);
    set_value( boost::filesystem::path( val), silent_edit);
}

void file_param_t::do_write( serialization::yaml_oarchive_t& out) const
{
    out << YAML::Key << "value"
            << YAML::Value << YAML::DoubleQuoted
            << filesystem::file_string( get_value<boost::filesystem::path>( *this));
}

void file_param_t::do_convert_relative_paths( const boost::filesystem::path& old_base, const boost::filesystem::path& new_base)
{
    boost::filesystem::path p( get_value<boost::filesystem::path>( *this));

    if( p.is_relative())
    {
        value().assign( filesystem::convert_relative_path( p, old_base, new_base));
        //update_widgets();
    }
}

void file_param_t::do_make_paths_absolute()
{
/*
    RAMEN_ASSERT( composition());

    boost::filesystem::path p( get_value<boost::filesystem::path>( *this));

    if( p.is_relative())
    {
        value().assign( composition()->relative_to_absolute( p));
        update_widgets();
    }
    */
}

void file_param_t::do_make_paths_relative()
{
/*
    RAMEN_ASSERT( composition());

    boost::filesystem::path p( get_value<boost::filesystem::path>( *this));

    if( p.is_absolute())
    {
        value().assign( composition()->absolute_to_relative( p));
        update_widgets();
    }
    */
}

} // namespace
} // namespace
