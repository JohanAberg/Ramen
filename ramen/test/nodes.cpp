// Copyright (c) 2012 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/python/python.hpp>

#include<ramen/test/test.hpp>

#include<ramen/app/application.hpp>
#include<ramen/app/document.hpp>

#include<ramen/test/test_node.hpp>

/*
namespace ramen
{
namespace nodes
{

class mock_node_t : public node_t
{
public:

    static const class_metadata_t& mock_node_class_metadata();
    virtual const class_metadata_t *class_metadata() const;

    mock_node_t() {}
    mock_node_t( const mock_node_t& other) : node_t( other) {}

    virtual node_t *do_clone() const
    {
        return new mock_node_t( *this);
    }

    MOCK_METHOD0( do_create_plugs, void());
    MOCK_METHOD0( do_create_params, void());
    MOCK_METHOD0( do_create_manipulators, void());
};

// factory
node_t *create_mock_node() { return new mock_node_t();}

const class_metadata_t *mock_node_t::class_metadata() const { return &mock_node_class_metadata();}

const class_metadata_t& mock_node_t::mock_node_class_metadata()
{
    static bool inited( false);
    static class_metadata_t info;

    if( !inited)
    {
        info.id = "builtin.mock";
        info.major_version = 1;
        info.minor_version = 0;
        info.menu = "Image";
        info.submenu = "Test";
        info.menu_item = "Mock";
        info.create = &create_mock_node;
        inited = true;
    }

    return info;
}

} // nodes
} // ramen
*/

void test_mock_node() {}

static bool registered1 = RAMEN_REGISTER_TEST_CASE( test_mock_node);

void test_nodes()
{
    ramen::nodes::factory_t::instance().register_node( ramen::nodes::test_node_t::test_node_class_metadata());

    // TODO: write this.
}

static bool registered2 = RAMEN_REGISTER_TEST_CASE( test_nodes);
