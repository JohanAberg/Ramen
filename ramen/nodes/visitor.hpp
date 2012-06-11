// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_NODES_VISITOR_HPP
#define	RAMEN_NODES_VISITOR_HPP

#include<ramen/config.hpp>

#include<ramen/nodes/node_fwd.hpp>

namespace ramen
{
namespace nodes
{

class visitor_t
{
public:

    virtual ~visitor_t();

    virtual void visit( node_t *n) = 0;
    virtual void visit( composite_node_t *n) = 0;
};

class generic_visitor_t : public visitor_t
{
public:

    virtual ~generic_visitor_t();

    virtual void visit( node_t *n) = 0;
    virtual void visit( composite_node_t *n);
};

} // namespace
} // namespace

#endif
