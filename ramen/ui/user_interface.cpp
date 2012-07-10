// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/user_interface.hpp>

#include<ramen/assert.hpp>

namespace ramen
{
namespace ui
{

user_interface_t::user_interface_t( boost::python::object obj)
{
    RAMEN_ASSERT( !obj.is_none());
    impl_ = obj;
}

void user_interface_t::fatal_error( const std::string& msg) const
{
}

void user_interface_t::error( const std::string& msg) const
{
}

void user_interface_t::inform( const std::string& msg) const
{
}

bool user_interface_t::question( const std::string& what, bool default_answer) const
{
    return default_answer;
}

} // namespace
} // namespace
