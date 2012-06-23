// Copyright (c) 2012 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_NODES_WORLD_NODE_HPP
#define RAMEN_NODES_WORLD_NODE_HPP

#include<ramen/nodes/composite_node.hpp>

#include<ramen/dependency/graph.hpp>

#include<ramen/image/format.hpp>

#include<ramen/ocio/context.hpp>

namespace ramen
{
namespace nodes
{

/**
\ingroup nodes
\brief A top level node that contains other nodes.
*/
class world_node_t : public composite_node_t
{
public:

    world_node_t();
    virtual ~world_node_t();

    /// Returns a const reference to the dependency graph.
    const dependency::graph_t& dependency_graph() const { return dep_graph_;}

    /// Returns a reference to the dependency graph.
    dependency::graph_t& dependency_graph() { return dep_graph_;}

    boost::signals2::signal<void ( node_t*)> node_added;
    boost::signals2::signal<void ( node_t*)> node_released;

    // OpenColorIO
    const ocio::context_t& ocio_context() const { return ocio_context_;}
    ocio::context_t& ocio_context()             { return ocio_context_;}

    // image formats.
    image::format_t default_image_format() const             { return default_img_format_;}
    void set_default_image_format( const image::format_t& f) { default_img_format_ = f;}

protected:

    world_node_t( const world_node_t& other);
    void operator=( const world_node_t&);

private:

    void emit_node_release_signal( node_t& n);

    dependency::graph_t dep_graph_;

    ocio::context_t ocio_context_;
    image::format_t default_img_format_;
};

} // namespace
} // namespace

#endif
