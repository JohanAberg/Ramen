// Copyright (c) 2012 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/name.hpp>

#include<algorithm>

#include<boost/swap.hpp>
#include<boost/thread/once.hpp>

#include<tbb/concurrent_unordered_set.h>

namespace ramen
{
namespace
{

struct name_pool_t
{
    typedef tbb::concurrent_unordered_set<std::string> name_set_type;
    typedef name_set_type::const_iterator	const_iterator;
    typedef name_set_type::iterator			iterator;

    static name_pool_t& instance()
    {
        static name_pool_t pool;
        return pool;
    }

    static void init() { instance().add( std::string(""));}

    const std::string *add( const std::string& str)
    {
        const_iterator it( names_.find( str));

        if( it != names_.end())
            return &(*it);

        std::pair<iterator, bool> result = names_.insert( str);
        return &(*result.first);
    }

    name_set_type names_;
};

boost::once_flag name_pool_init_flag;
std::string g_empty_string( "");

} // unnamed

name_t::name_t() { init( g_empty_string);}

name_t::name_t( const char *str)
{
    if( str)
        init( std::string( str));
    else
        init( g_empty_string);
}

name_t::name_t( const std::string& str)	{ init( str);}

void name_t::init( const std::string& str)
{
    boost::call_once( &name_pool_t::init, name_pool_init_flag);
    data_ = name_pool_t::instance().add( str);
}

name_t& name_t::operator=( const name_t& other)
{
    name_t tmp( other);
    boost::swap( *this, tmp);
    return *this;
}

} // ramen
