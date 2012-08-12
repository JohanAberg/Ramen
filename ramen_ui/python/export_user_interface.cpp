// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen_ui/config.hpp>

#include<boost/python.hpp>
namespace bpy = boost::python;

#include<ramen_ui/user_interface.hpp>
using namespace ramen_ui;

namespace
{

struct user_interface_wrap_t : user_interface_t, bpy::wrapper<user_interface_t>
{
    void fatal_error( const std::string& msg) const
    {
        this->get_override( "fatal_error")( msg);
    }

    void error( const std::string& msg) const
    {
        this->get_override( "error")( msg);
    }

    void inform( const std::string& msg) const
    {
        this->get_override( "inform")( msg);
    }

    bool question( const std::string& what, bool default_answer) const
    {
        return this->get_override( "inform")( what, default_answer);
    }
};

} // unnamed

void export_user_interface()
{
    bpy::class_<user_interface_wrap_t, boost::noncopyable>( "base_user_interface")
        .def( "fatal_error", bpy::pure_virtual( &user_interface_t::fatal_error))
        .def( "error", bpy::pure_virtual( &user_interface_t::error))
        .def( "inform", bpy::pure_virtual( &user_interface_t::inform))
        .def( "question", bpy::pure_virtual( &user_interface_t::question))
        ;
}
