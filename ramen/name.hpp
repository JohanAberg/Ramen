// Copyright (c) 2012 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef BASE_NAME_HPP
#define BASE_NAME_HPP

#include<string>
#include<algorithm>

#include<boost/type_traits/has_nothrow_constructor.hpp>
#include<boost/type_traits/has_nothrow_copy.hpp>
#include<boost/type_traits/has_nothrow_assign.hpp>
#include<boost/type_traits/has_nothrow_destructor.hpp>

namespace ramen
{

class name_t
{
public:

    name_t();
    explicit name_t( const char *str);
    explicit name_t( const std::string& str);

    name_t& operator=( const name_t& other);

    operator const std::string&() const { return *data_;}

    const char *c_str() const { return data_->c_str();}

    bool operator==( const name_t& other) const { return data_ == other.data_;}
    bool operator!=( const name_t& other) const { return data_ != other.data_;}
    bool operator<( const name_t& other) const	{ return data_ < other.data_;}

private:

    void init( const std::string& str);

    friend inline void swap( name_t& x, name_t& y)
    {
        std::swap( x.data_, y.data_);
    }

    const std::string *data_;
};

} // ramen

namespace boost
{

template<> struct has_nothrow_constructor<ramen::name_t> : public true_type {};
template<> struct has_nothrow_copy<ramen::name_t>		: public true_type {};
template<> struct has_nothrow_assign<ramen::name_t>		: public true_type {};
template<> struct has_nothrow_destructor<ramen::name_t>	: public true_type {};

} // boost

#endif
