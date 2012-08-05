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

    nodes::world_node_t *world = &( app().document());

    world->node_added.connect( node_added);
    world->node_released.connect( node_released);

    // check that world does not have a parent node.
    BOOST_CHECK( !world->parent_node());

    // try to create a non-existant node.
    ramen::nodes::node_t *null_node = world->create_node_by_id( "builtin.no_node");
    BOOST_CHECK( null_node == 0);

    // create a node, check that it's correctly inserted as a child of world.
    ramen::nodes::node_t *node = world->create_node_by_id( "builtin.test");
    BOOST_CHECK( node);
    BOOST_CHECK( world->nodes().size() == 1);
    BOOST_CHECK( node->parent_node() == world);

    // create a second node.
    ramen::nodes::node_t *node2 = world->create_node_by_id( "builtin.test");
    BOOST_CHECK( node2);

    // check that node2 has been renamed.
    BOOST_CHECK( node->name() != node2->name());

    // test find_node
    BOOST_CHECK( world->find_node( node->name()) == node);
    BOOST_CHECK( world->find_node( node2->name()) == node2);
    BOOST_CHECK( world->find_node( "null node") == 0);

    // test clone
    std::auto_ptr<ramen::nodes::node_t> node3_ptr( nodes::new_clone( *node2 ));
    ramen::nodes::node_t *node3 = node3_ptr.get();
    world->add_node( node3_ptr);
    BOOST_CHECK( node3->name() != node2->name());
    BOOST_CHECK( node3->name() != node->name());

    // test connections
    {
        world->connect( node, base::name_t( "result"),
                        node2, base::name_t( "front"));

        BOOST_CHECK( node->output_plug( base::name_t( "result")).connections().size() == 1 );
        BOOST_CHECK( node2->input( base::name_t( "front")) == node);

        world->connect( node, base::name_t( "result"),
                        node3, base::name_t( "back"));

        BOOST_CHECK( node->output_plug( base::name_t( "result")).connections().size() == 2 );
        BOOST_CHECK( node3->input( base::name_t( "back")) == node);

        world->connect( node3, base::name_t( "result"),
                        node2, base::name_t( "front"));

        world->disconnect( node3, base::name_t( "result"),
                            node2, base::name_t( "front"));
    }

    // check # of calls
    nodes::test_node_t *test_node = dynamic_cast<nodes::test_node_t*>( node);
    BOOST_CHECK( test_node->init_calls == 1);
    BOOST_CHECK( test_node->create_plugs_calls == 1);
    BOOST_CHECK( test_node->create_params_calls == 1);
    BOOST_CHECK( test_node->create_manips_calls == 1);

    // delete all nodes.
    app().create_new_document();

    // check that
    BOOST_CHECK( node_added_calls == 3);
    BOOST_CHECK( node_added_calls == node_released_calls);



    nodes::factory_t::instance().unregister_all();
}

static bool registered1 = RAMEN_REGISTER_TEST_CASE( test_nodes);
