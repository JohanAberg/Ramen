// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/test/test.hpp>

#include<map>

#include<gmock/gmock.h>

#include<ramen/assert.hpp>

namespace
{

class test_registry_t
{
public:

    typedef std::map<std::string, boost::function<void()> > map_type;
    typedef map_type::iterator iterator;
    typedef map_type::const_iterator const_iterator;

    static test_registry_t& instance()
    {
        static test_registry_t reg;
        return reg;
    }

    bool register_test( const char *name, const boost::function<void()>& fun)
    {
        std::string n( name);
        std::pair<std::map<std::string, boost::function<void()> >::iterator, bool> result = tests_.insert( std::make_pair( n, fun));
        RAMEN_ASSERT( result.second);
        return true;
    }

    const_iterator begin() const    { return tests_.begin();}
    const_iterator end() const      { return tests_.end();}

private:

    test_registry_t() {}
    ~test_registry_t() {}

    map_type tests_;
};

} // unnamed

bool do_register_ramen_test( const char *name, const boost::function<void()>& fun)
{
    return test_registry_t::instance().register_test( name, fun);
}

bool init_tests()
{
    // builtin tests, if any.
    // framework::master_test_suite().add( BOOST_TEST_CASE( &basic_tests));

    // add registered tests.
    for( test_registry_t::const_iterator it( test_registry_t::instance().begin()), e( test_registry_t::instance().end()); it != e; ++it)
        framework::master_test_suite().add( boost::unit_test::make_test_case( boost::unit_test::callback0<>(it->second), it->first.c_str()));

    return true;
}

int run_ramen_unit_tests( int *argc, char **argv)
{
    // init gmock
    ::testing::GTEST_FLAG(throw_on_failure) = true;
    ::testing::InitGoogleMock( argc, argv);

    // run tests
    return ::boost::unit_test::unit_test_main( &init_tests, *argc, argv);
}
