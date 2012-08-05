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
#include<boost/optional.hpp>
#include<boost/foreach.hpp>
#include<boost/range/algorithm/for_each.hpp>

#include<base/container/ptr_vector_util.hpp>

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
        connection_type( node_t *s, const base::name_t& splug, node_t *d, const base::name_t& dplug);

        bool operator==( const connection_type& other) const;
        bool operator!=( const connection_type& other) const { return !(*this == other);}

        node_t *src;
        base::name_t src_plug;

        node_t *dst;
        base::name_t dst_plug;
    };

    typedef boost::ptr_vector<node_t> node_container_type;

    typedef node_container_type::iterator       node_iterator;
    typedef node_container_type::const_iterator	const_node_iterator;

    typedef node_container_type::reverse_iterator       reverse_node_iterator;
    typedef node_container_type::const_reverse_iterator	const_reverse_node_iterator;

    typedef node_container_type         node_range_type;
    typedef const node_container_type	const_node_range_type;

    typedef std::vector<connection_type> connection_container_type;

    typedef connection_container_type::iterator       connection_iterator;
    typedef connection_container_type::const_iterator const_connection_iterator;

    typedef connection_container_type::reverse_iterator       reverse_connection_iterator;
    typedef connection_container_type::const_reverse_iterator const_reverse_connection_iterator;

    typedef connection_container_type       connection_range_type;
    typedef const connection_container_type	const_connection_range_type;

    // ranges
    node_range_type& nodes()                { return nodes_;}
    const_node_range_type& nodes() const    { return nodes_;}

    const_connection_range_type& connections() const    { return connections_;}
    connection_range_type& connections()                { return connections_;}

private:

    graph_t();
    graph_t( const graph_t& other);

    void add_node( std::auto_ptr<node_t> n);
    std::auto_ptr<node_t> release_node( node_t *n);

    void add_connection( const connection_type& c);

    template<class InputRange>
    void add_connections( const InputRange& range)
    {
        boost::range::for_each( range, boost::bind( &graph_t::add_connection, this, _1));
    }

    void remove_connection( const connection_type& c);

    template<class InputRange>
    void remove_connections( const InputRange& range)
    {
        boost::range::for_each( range, boost::bind( &graph_t::remove_connection, this, _1));
    }

    bool node_is_connected( const node_t *n) const;

    template<class OutputIter>
    void node_connections( const node_t *n, OutputIter it) const
    {
        RAMEN_ASSERT( n);
        RAMEN_ASSERT( base::container::contains_ptr( n, nodes()));

        BOOST_FOREACH( const connection_type& c, connections())
        {
            if( c.src == n || c.dst == n)
                *it++ = c;
        }
    }

    boost::optional<connection_type> find_connection( const node_t *dst, const base::name_t& dst_plug) const;

    friend class composite_node_t;

    void operator=( const graph_t& other);

    node_container_type nodes_;
    connection_container_type connections_;
};

} // namespace
} // namespace

#endif
