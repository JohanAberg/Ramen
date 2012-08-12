// Copyright (c) 2012 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_USER_INTERFACE_HPP
#define RAMEN_UI_USER_INTERFACE_HPP

#include<ramen_ui/user_interface_fwd.hpp>

#include<ramen_ui/config.hpp>

#include<string>

#include<boost/noncopyable.hpp>

namespace ramen_ui
{

class RAMEN_UI_API user_interface_t : boost::noncopyable
{
public:

    user_interface_t();

    virtual void fatal_error( const std::string& msg) const = 0;
    virtual void error( const std::string& msg) const = 0;
    virtual void inform( const std::string& msg) const = 0;
    virtual bool question( const std::string& what, bool default_answer) const = 0;
};

} // namespace

#endif
