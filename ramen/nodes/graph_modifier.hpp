// Copyright (c) 2012 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_NODES_GRAPH_MODIFIER_HPP
#define RAMEN_NODES_GRAPH_MODIFIER_HPP

#include<boost/noncopyable.hpp>

#include<memory>

#include<ramen/nodes/graph.hpp>

namespace ramen
{
namespace nodes
{

class graph_modifier_t : boost::noncopyable
{
public:

    explicit graph_modifier_t( bool undoable = true);
    ~graph_modifier_t();

private:

    struct command_impl;
    std::auto_ptr<command_impl> command_;
};

} // namespace
} // namespace

#endif
