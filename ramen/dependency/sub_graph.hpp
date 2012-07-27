// Copyright (c) 2012 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_DEPENDENCY_SUB_GRAPH_HPP
#define RAMEN_DEPENDENCY_SUB_GRAPH_HPP

#include<ramen/config.hpp>

#include<ramen/dependency/graph_fwd.hpp>
#include<ramen/dependency/node_fwd.hpp>

#include<vector>

#include<boost/noncopyable.hpp>
#include<boost/function.hpp>

#include<ramen/dependency/dependency.hpp>

namespace ramen
{
namespace dependency
{

/*!
\ingroup depgraph
\brief Part of Ramen's dependency graph.
*/
class RAMEN_API sub_graph_t : boost::noncopyable
{
public:

	sub_graph_t();
	virtual ~sub_graph_t();

    const std::vector<node_t*>& nodes() const
    {
        return nodes_;
    }

    /// Adds a new dependency node to this subgraph
    void add_dependency_node( node_t *n);

    /// Adds a new dependency between two nodes.
    void add_dependency( node_t *src, node_t *dst);

    /// Propagates the dirty flag.
    void propagate_dirty_flags();

    /// Applies a function f to each child sub_graph.
    virtual void for_each_sub_graph( const boost::function<void( sub_graph_t*)> *f);

private:

    friend class dependency::graph_t;

    /// Clears all nodes dirty flag.
    void clear_all_dirty();

    /// Calls node_t::notify for each dirty node.
    void notify_all_dirty();

    /*!
        \brief Customization hook for sub_graph_t::propagate_dirty_flags.
        For subclasses to implement.
    */
    virtual void do_propagate_dirty_flags();

    std::vector<node_t*> nodes_;
    std::vector<dependency_t> dependencies_;
};

} // dependency
} // ramen

#endif
