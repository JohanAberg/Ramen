// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_NODES_EDGE_HPP
#define RAMEN_NODES_EDGE_HPP

#include<ramen/nodes/graph_color.hpp>
#include<ramen/nodes/node_fwd.hpp>

#include<ramen/name.hpp>

namespace ramen
{

struct edge_t
{
    edge_t();
    edge_t( node_t *s=0, node_t *d=0, int p=-1);
    edge_t( node_t *s, const name_t& splug, node_t *d, const name_t& dplug);

    bool operator==( const edge_t& other) const;
    bool operator!=( const edge_t& other) const { return !(*this == other);}

    node_t *src;
    name_t src_plug;

    node_t *dst;
    name_t dst_plug;

    int port;

    // for graph algorithms
    graph_color_t graph_color() const            { return graph_color_;}
    void set_graph_color( graph_color_t c) const { graph_color_ = c;}

    mutable graph_color_t graph_color_;
};

} // namespace

#endif
