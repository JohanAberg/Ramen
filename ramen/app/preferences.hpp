// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_APP_PREFERENCES_HPP
#define RAMEN_APP_PREFERENCES_HPP

#include<ramen/config.hpp>

#include<ramen/python/python.hpp>

#include<ramen/app/preferences_fwd.hpp>

#include<vector>
#include<string>

#include<boost/noncopyable.hpp>
#include<boost/python.hpp>

#include<ramen/app/application_fwd.hpp>

namespace ramen
{

/**
\ingroup app
\brief contains the user preferences
*/
class preferences_t : boost::noncopyable
{
public:

private:

    friend class ramen::application_t;

    explicit preferences_t( boost::python::object obj);

    boost::python::object impl_;
};

} // namespace

#endif
