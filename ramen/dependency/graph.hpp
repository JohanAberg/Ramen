// Copyright (c) 2012 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_DEPENDENCY_GRAPH_HPP
#define RAMEN_DEPENDENCY_GRAPH_HPP

#include<ramen/config.hpp>

#include<ramen/dependency/graph_fwd.hpp>

#include<vector>
#include<map>

#include<boost/noncopyable.hpp>
#include<boost/function.hpp>

#include<ramen/dependency/node.hpp>
#include<ramen/dependency/exceptions.hpp>

#include<ramen/nodes/node_fwd.hpp>

namespace ramen
{
namespace dependency
{

/*!
\ingroup depgraph
\brief Ramen's dependency graph.
*/
class RAMEN_API graph_t : boost::noncopyable
{
public:

	explicit graph_t( nodes::world_node_t *w);

    /// Clears all nodes dirty flag.
    void clear_all_dirty();

    /// Calls notify for all dirty nodes on the dep. graph.
    void notify_all_dirty();

private:

    void for_each_sub_graph( const boost::function<void( sub_graph_t*)>& f);

    static void clear_sub_graph_dirty( sub_graph_t *g);
    static void notify_sub_graph( sub_graph_t *g);

    nodes::world_node_t *world_;
};

} // dependency
} // ramen

#endif
