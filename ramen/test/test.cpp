// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/test/unit_test.hpp>
using namespace boost::unit_test;

// prototypes
void register_expression_tests();
void register_python_tests();

bool init_tests()
{
    register_expression_tests();
    register_python_tests();
	return true;
}

int run_ramen_unit_tests( int argc, char **argv)
{
	return ::boost::unit_test::unit_test_main( &init_tests, argc, argv);
}
