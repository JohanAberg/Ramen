// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/params/combo_group_param.hpp>

#include<algorithm>

#include<boost/foreach.hpp>
#include<boost/range/algorithm/for_each.hpp>

#include<QStackedWidget>
#include<QComboBox>
#include<QVBoxLayout>
#include<QLabel>

#include<ramen/app/application.hpp>

#include<ramen/nodes/node.hpp>

#include<ramen/params/static_param_command.hpp>

#include<ramen/serialization/yaml_oarchive.hpp>

#include<ramen/ui/user_interface.hpp>
#include<ramen/ui/inspector/inspector.hpp>

namespace ramen
{
namespace params
{

combo_group_param_t::combo_group_param_t( const std::string& name) : composite_param_t( name)
{
    set_default_value( 0);
}

combo_group_param_t::combo_group_param_t( const combo_group_param_t& other) : composite_param_t( other)
{
}

void combo_group_param_t::set_default_value( int x)
{
    value().assign( x);
}

void combo_group_param_t::set_value( int x, change_reason reason)
{
    if( can_undo())
        param_set()->add_command( this);

    value().assign( x);
    emit_param_changed( reason);
}

std::auto_ptr<undo::command_t> combo_group_param_t::do_create_command()
{
    return std::auto_ptr<undo::command_t>( new static_param_command_t( *param_set(), id()));
}

void combo_group_param_t::do_add_to_hash( hash::generator_t& hash_gen) const
{
    int val = get_value<int>( *this);
    hash_gen << params()[val].name();
    params()[val].add_to_hash( hash_gen);
}

boost::python::object combo_group_param_t::to_python( const base::poly_regular_t& v) const
{
    return boost::python::object( v.cast<int>());
}

base::poly_regular_t combo_group_param_t::from_python( const boost::python::object& obj) const
{
    int i = boost::python::extract<int>( obj);
    return base::poly_regular_t( i);
}

// serialization
void combo_group_param_t::do_read( const serialization::yaml_node_t& node)
{
    /*
    serialization::yaml_node_t n = node.get_node( "value");
    std::string val;
    n >> val;

    for( int i = 0; i < params().size(); ++i)
    {
        if( params()[i].name() == val)
        {
            value().assign( i);
            return;
        }
    }

    node.error_stream() << "Unknown string in combo group param\n";
    */
}

void combo_group_param_t::do_write( serialization::yaml_oarchive_t& out) const
{
    /*
    int v = get_value<int>( *this);
    out << YAML::Key << "value" << YAML::Value;
    out << params()[v].name();
    out.check_errors();
    */
}

} // namespace
} // namespace
