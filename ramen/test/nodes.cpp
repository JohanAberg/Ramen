// Copyright (c) 2012 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/python/python.hpp>

#include<ramen/test/test.hpp>

#include<ramen/app/application.hpp>
#include<ramen/app/document.hpp>

#include<ramen/test/test_node.hpp>
using namespace ramen;

// test signals
int node_added_calls = 0;
int node_released_calls = 0;

void node_added( nodes::node_t*)       { node_added_calls++;}
void node_released( nodes::node_t*)    { node_released_calls++;}

void test_nodes()
{
    nodes::factory_t::instance().register_node( ramen::nodes::test_node_t::test_node_class_metadata());

    // start from new.
    app().create_new_document();

    app().document().node_added.connect( node_added);
    app().document().node_released.connect( node_released);

    // check that world does not have a parent node.
    BOOST_CHECK( !app().document().parent_node());

    // try to create a non-existant node.
    ramen::nodes::node_t *null_node = app().document().create_node_by_id( "builtin.no_node");
    BOOST_CHECK( null_node == 0);

    // create a node, check that it's correctly inserted as a child of world.
    ramen::nodes::node_t *node = app().document().create_node_by_id( "builtin.test");
    BOOST_CHECK( node);
    BOOST_CHECK( app().document().graph().nodes().size() == 1);
    BOOST_CHECK( node->parent_node() == &app().document());

    // create a second node.
    ramen::nodes::node_t *node2 = app().document().create_node_by_id( "builtin.test");
    BOOST_CHECK( node2);

    // check that node2 has been renamed.
    BOOST_CHECK( node->name() != node2->name());

    // test find_node
    BOOST_CHECK( app().document().find_node( node->name()) == node);
    BOOST_CHECK( app().document().find_node( node2->name()) == node2);
    BOOST_CHECK( app().document().find_node( "null node") == 0);

    // delete all nodes.
    app().create_new_document();

    // check that
    BOOST_CHECK( node_added_calls == 2);
    BOOST_CHECK( node_added_calls == node_released_calls);

    nodes::factory_t::instance().unregister_all();
}

static bool registered1 = RAMEN_REGISTER_TEST_CASE( test_nodes);
