// Copyright (c) 2011 Esteban Tovagliari

#include<ramen/test/unit_test.hpp>
using namespace boost::unit_test;

#include<ramen/expressions/test.hpp>

#include<ramen/python/test.hpp>

bool init_tests()
{
	framework::master_test_suite().add( BOOST_TEST_CASE( &run_expression_tests));
	framework::master_test_suite().add( BOOST_TEST_CASE( &run_python_tests));
	return true;
}

int run_unit_tests( int argc, char **argv)
{
	return ::boost::unit_test::unit_test_main( &init_tests, argc, argv);
}
