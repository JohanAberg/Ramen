// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_NODES_EDGE_HPP
#define RAMEN_NODES_EDGE_HPP

#include<ramen/nodes/graph_color.hpp>
#include<ramen/nodes/node_fwd.hpp>

namespace ramen
{

struct edge_t
{
    edge_t( node_t *s=0, node_t *d=0, int p=-1) : src(s), dst(d), port(p) {}
	
    bool operator==( const edge_t& other) const { return src == other.src && dst == other.dst && port == other.port;}
    bool operator!=( const edge_t& other) const { return !(src == other.src && dst == other.dst && port == other.port);}

    node_t *src;
    node_t *dst;
    int port;
	
	// for graph algorithms
    graph_color_t graph_color() const            { return graph_color_;}
    void set_graph_color( graph_color_t c) const { graph_color_ = c;}
	
    mutable graph_color_t graph_color_;
};

} // namespace

#endif
