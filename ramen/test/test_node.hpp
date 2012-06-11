// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_TEST_TEST_NODE_HPP
#define RAMEN_TEST_TEST_NODE_HPP

#include<ramen/nodes/node.hpp>

namespace ramen
{
namespace nodes
{

class test_node_t : public node_t
{
public:

    static const class_metadata_t& test_node_class_metadata();
    virtual const class_metadata_t *class_metadata() const;

protected:

    test_node_t();

    test_node_t( const test_node_t& other);
    void operator=( const test_node_t& other);

private:

    virtual node_t *do_clone() const;
    virtual void do_create_plugs();
    virtual void do_create_params();
    virtual void do_create_manipulators();

    friend node_t *create_test_node();
};

} // nodes
} // ramen

#endif
