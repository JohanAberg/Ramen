// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_USER_INTERFACE_HPP
#define RAMEN_UI_USER_INTERFACE_HPP

#include<ramen/config.hpp>

#include<ramen/python/python.hpp>

#include<ramen/ui/user_interface_fwd.hpp>

#include<boost/python.hpp>

#include<ramen/app/application_fwd.hpp>

namespace ramen
{
namespace ui
{

/**
\ingroup ui
\brief user interface class.
*/
struct RAMEN_API user_interface_t
{
public:

    // error reporting
    void fatal_error( const std::string& msg) const;
    void error( const std::string& msg) const;
    void inform( const std::string& msg) const;
    bool question( const std::string& what, bool default_answer) const;

private:

    friend class ramen::application_t;

    explicit user_interface_t( boost::python::object obj);

    boost::python::object impl_;
};

} // namespace
} // namespace

#endif
