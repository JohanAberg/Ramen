// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/test/test_node.hpp>

namespace ramen
{
namespace nodes
{

test_node_t::test_node_t() {}

test_node_t::test_node_t( const test_node_t& other) : node_t( other) {}

node_t *test_node_t::do_clone() const { return new test_node_t( *this);}

void test_node_t::do_create_plugs() {}
void test_node_t::do_create_params() {}
void test_node_t::do_create_manipulators() {}

// factory
node_t *create_test_node() { return new test_node_t();}

const class_metadata_t *test_node_t::class_metadata() const { return &test_node_class_metadata();}

const class_metadata_t& test_node_t::test_node_class_metadata()
{
    static bool inited( false);
    static class_metadata_t info;

    if( !inited)
    {
        info.id = "builtin.test";
        info.major_version = 1;
        info.minor_version = 0;
        info.menu = "Image";
        info.submenu = "Test";
        info.menu_item = "Test";
        info.create = &create_test_node;
        inited = true;
    }

    return info;
}

} // nodes
} // ramen
