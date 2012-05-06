// Copyright (c) 2012 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_NODES_COMPOSITE_NODE_HPP
#define RAMEN_NODES_COMPOSITE_NODE_HPP

#include<ramen/nodes/node.hpp>

#include<boost/scoped_ptr.hpp>

#include<ramen/nodes/node_graph.hpp>
#include<ramen/nodes/node_visitor.hpp>

#include<ramen/ui/graph_layout_fwd.hpp>

namespace ramen
{

/**
\ingroup nodes
\brief Abstract base class for nodes that contain other nodes.
*/
class composite_node_t : public node_t
{
public:

	virtual ~composite_node_t();

	/// Dispatch function for visitor pattern.
	virtual void accept( node_visitor& v);

    /// Returns a const reference to the node graph inside this node.
    const node_graph_t& graph() const { return g_;}

    /// Returns a reference to the node graph inside this node.
    node_graph_t& graph() { return g_;}

	typedef node_graph_t::const_node_iterator const_iterator;

	/// Returns a const iterator to the first children node.
	const_iterator begin() const { return g_.nodes_begin();}

	/// Returns a const end iterator.
	const_iterator end() const { return g_.nodes_end();}

	typedef node_graph_t::node_iterator iterator;

    /// Returns an iterator to the first children node.
	iterator begin() { return g_.nodes_begin();}

	/// Returns an end iterator.
	iterator end() { return g_.nodes_end();}

    /// Adds a node as a child of this node.
    virtual void add_node( std::auto_ptr<node_t> n);

    /// Removes a child node from this node.
    virtual std::auto_ptr<node_t> remove_node( node_t *n);

protected:

	composite_node_t();

	composite_node_t( const composite_node_t& other);
	void operator=( const composite_node_t&);

private:

	node_graph_t g_;
	boost::scoped_ptr<ui::graph_layout_t> layout_;
};

} // namespace

#endif
