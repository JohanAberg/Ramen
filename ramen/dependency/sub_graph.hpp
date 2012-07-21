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

#include<ramen/dependency/relation.hpp>

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

private:

    std::vector<node_t*> nodes_;
    std::vector<relation_t> dependencies_;
};

} // dependency
} // ramen

#endif
