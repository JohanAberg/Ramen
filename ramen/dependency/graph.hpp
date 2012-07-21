// Copyright (c) 2012 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_DEPENDENCY_GRAPH_HPP
#define RAMEN_DEPENDENCY_GRAPH_HPP

#include<ramen/config.hpp>

#include<ramen/dependency/graph_fwd.hpp>

#include<vector>
#include<map>

#include<boost/graph/adjacency_list.hpp>

#include<ramen/dependency/node.hpp>
#include<ramen/dependency/exceptions.hpp>

namespace ramen
{
namespace dependency
{

/*!
\ingroup depgraph
\brief Ramen's dependency graph.
*/
class RAMEN_API graph_t
{
public:

	graph_t();

private:

    std::vector<sub_graph_t*> sub_graphs_;
};

} // dependency
} // ramen

#endif
