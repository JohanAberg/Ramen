// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_NODES_GRAPH_HPP
#define RAMEN_NODES_GRAPH_HPP

#include<ramen/nodes/graph_fwd.hpp>

#include<algorithm>
#include<memory>
#include<vector>
#include<map>

#include<boost/bind.hpp>
#include<boost/ptr_container/ptr_vector.hpp>

#include<ramen/nodes/node.hpp>

namespace ramen
{
namespace nodes
{

/**
\ingroup nodes
\brief A collection of nodes and connections between them.
*/
class graph_t
{
public:

    struct connection_type
    {
        connection_type();
        connection_type( node_t *s, const name_t& splug, node_t *d, const name_t& dplug);

        bool operator==( const connection_type& other) const;
        bool operator!=( const connection_type& other) const { return !(*this == other);}

        node_t *src;
        name_t src_plug;

        node_t *dst;
        name_t dst_plug;

        // for graph algorithms
        graph_color_t graph_color() const            { return graph_color_;}
        void set_graph_color( graph_color_t c) const { graph_color_ = c;}

        mutable graph_color_t graph_color_;
    };

    typedef boost::ptr_vector<node_t> node_container_type;

    typedef node_container_type::iterator       node_iterator;
    typedef node_container_type::const_iterator	const_node_iterator;

    typedef node_container_type::reverse_iterator       reverse_node_iterator;
    typedef node_container_type::const_reverse_iterator	const_reverse_node_iterator;

    typedef node_container_type         node_range_type;
    typedef const node_container_type	const_node_range_type;

    typedef std::vector<connection_type>::iterator       connection_iterator;
    typedef std::vector<connection_type>::const_iterator const_connection_iterator;

    typedef std::vector<connection_type>::reverse_iterator       reverse_connection_iterator;
    typedef std::vector<connection_type>::const_reverse_iterator const_reverse_connection_iterator;

    typedef std::vector<connection_type>       connection_range_type;
    typedef const std::vector<connection_type>	const_connection_range_type;

protected:

    graph_t();
    graph_t( const graph_t& other);

    void add_node( std::auto_ptr<node_t> n);
    std::auto_ptr<node_t> release_node( node_t *n);

    void add_connection( const connection_type& e);
    void remove_connection( const connection_type& e);

public:

    // iterators & ranges
    node_iterator nodes_begin() { return nodes_.begin();}
    node_iterator nodes_end()   { return nodes_.end();}

    const_node_iterator nodes_begin() const { return nodes_.begin();}
    const_node_iterator nodes_end() const   { return nodes_.end();}

    node_range_type& nodes()                { return nodes_;}
    const_node_range_type& nodes() const    { return nodes_;}

    connection_iterator connections_begin() { return connections_.begin();}
    connection_iterator connections_end()   { return connections_.end();}

    const_connection_iterator connections_begin() const { return connections_.begin();}
    const_connection_iterator connections_end() const   { return connections_.end();}

    const_connection_range_type& connections() const    { return connections_;}
    connection_range_type& connections()                { return connections_;}

private:

    friend class composite_node_t;

    void operator=( const graph_t& other);

    node_container_type nodes_;
    std::vector<connection_type> connections_;
};

} // namespace
} // namespace

#endif
