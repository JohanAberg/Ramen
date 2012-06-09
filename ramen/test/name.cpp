// Copyright (c) 2012 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/test/test.hpp>

#include<vector>

#include<boost/thread.hpp>
#include<boost/lexical_cast.hpp>

#include<ramen/name.hpp>
using namespace ramen;

void single_name_test()
{
    name_t empty;
    name_t one( "one");
    name_t two( "two");

    name_t one2( "one");

    name_t s2( std::string( "two"));

    BOOST_CHECK( one == one2);
    BOOST_CHECK( one != two);

    BOOST_CHECK( s2 == two);

    name_t n3;
    n3 = two;
    BOOST_CHECK( n3 == s2);

    name_t empty2( std::string( ""));
    BOOST_CHECK( empty == empty2);
}

static bool registered1 = RAMEN_REGISTER_TEST_CASE( single_name_test);

void mult_name_thread_fun()
{
    for( int i = 0; i < 100; ++i)
    {
        name_t x;
        name_t y( "XYZ");
        name_t z( "ZXY");

        std::string num = boost::lexical_cast<std::string>( i);
        name_t n( num);
    }
}

// Not really a test...
void multi_name_test()
{
    boost::thread_group group;

    for( int i = 0; i < 5; ++i)
        group.create_thread( mult_name_thread_fun);

    group.join_all();
}

static bool registered2 = RAMEN_REGISTER_TEST_CASE( multi_name_test);
