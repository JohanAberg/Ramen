// Copyright (c) 2012 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_TYPEINFO_HPP
#define RAMEN_TYPEINFO_HPP

#include<ramen/config.hpp>

#include<typeinfo>
#include<string>

namespace ramen
{

class RAMEN_API bad_cast : public std::bad_cast
{
public:

    bad_cast();
    bad_cast( const std::type_info& from, const std::type_info& to);

    bad_cast( const bad_cast&);
    bad_cast& operator=( const bad_cast&);
    virtual ~bad_cast() throw();

    virtual const char* what() const throw();

private:

    std::string what_;
};


} // namespace

#endif
