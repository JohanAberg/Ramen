// Copyright (c) 2012 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/typeinfo.hpp>

namespace ramen
{

bad_cast::bad_cast() : what_( "bad_cast") {}

bad_cast::bad_cast( const bad_cast& error) : what_( error.what_) {}

bad_cast::bad_cast(const std::type_info& from, const std::type_info& to) : what_("bad_cast: ")
{
    what_ += from.name();
    what_ += " -> ";
    what_ += to.name();
}

bad_cast::~bad_cast() throw() {}

bad_cast& bad_cast::operator=(const bad_cast& error)
{
    what_ = error.what_;
    return *this;
}

const char* bad_cast::what() const throw() { return what_.c_str();}

} // ramen
