// Copyright (c) 2010 Esteban Tovagliari

#include<ramen/params/popup_param.hpp>

#include<QComboBox>
#include<QLabel>

#include<ramen/app/application.hpp>

#include<ramen/params/param_set.hpp>

#include<ramen/serialization/yaml_oarchive.hpp>

namespace ramen
{
namespace params
{

popup_param_t::popup_param_t( const std::string& name) : static_param_t( name)
{
    set_default_value( 0);
}

popup_param_t::popup_param_t( const popup_param_t& other) : static_param_t( other), menu_items_( other.menu_items_) {}

void popup_param_t::set_default_value( int x)
{
    value().assign( x);
}

void popup_param_t::set_value( int x, change_reason reason)
{
    if( can_undo())
        param_set()->add_command( this);

    value().assign( x);
    emit_param_changed( reason);
}

void popup_param_t::add_menu_item( const std::string& item) { menu_items_.push_back( item);}

void popup_param_t::do_add_to_hash( hash::generator_t& hash_gen) const
{
    hash_gen << menu_items()[ get_value<int>( *this)];
}

boost::python::object popup_param_t::to_python( const poly_param_value_t& v) const
{
    std::string str = menu_items()[v.cast<int>()];
    return boost::python::object( str);
}

poly_param_value_t popup_param_t::from_python( const boost::python::object& obj) const
{
    std::string str = boost::python::extract<std::string>( obj);
    int index = find_index_for_string( str);
    return poly_param_value_t( index);
}

void popup_param_t::do_read( const serialization::yaml_node_t& node)
{
    serialization::yaml_node_t n = node.get_node( "value");
    std::string val;
    n >> val;

    int index = find_index_for_string( val);

    if( index != -1)
        value().assign( index);
    else
    {
        node.error_stream() << "error: item " << val << " not found on param\n";
        value().assign( 0);
    }
}

void popup_param_t::do_write( serialization::yaml_oarchive_t& out) const
{
    int v = get_value<int>( *this);
    out << YAML::Key << "value" << YAML::Value;
    out << YAML::DoubleQuoted << menu_items()[v];
}

int popup_param_t::find_index_for_string( const std::string& s) const
{
    for( int i = 0; i < menu_items().size(); ++i)
    {
        if( menu_items()[i] == s)
            return i;
    }

    return -1;
}

} // namespace
} // namespace
