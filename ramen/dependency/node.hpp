// Copyright (c) 2012 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_DEPENDENCY_NODE_HPP
#define RAMEN_DEPENDENCY_NODE_HPP

#include<ramen/config.hpp>

#include<ramen/dependency/node_fwd.hpp>

#include<utility>
#include<vector>

#include<boost/signals2/signal.hpp>

#include<ramen/dependency/graph_fwd.hpp>

namespace ramen
{
namespace dependency
{

/*!
\ingroup depgraph
\brief A node in Ramen's dependency graph.
*/

class RAMEN_API node_t
{
public:

    node_t();
    virtual ~node_t();

    /// Returns true if this node is dirty.
    bool dirty() const;

    /// Sets this node dirty flag.
    void set_dirty();

protected:

    node_t( const node_t& other);

    /// Clears this node dirty flag.
    void clear_dirty();

    /// Notify observers that this node has changed.
    void notify();

private:

    friend class ramen::dependency::graph_t;
    friend class ramen::dependency::sub_graph_t;

    void operator=( const node_t& other);

    /*!
        \brief Customization hook for node_t::notify.
        For subclasses to implement.
    */
    virtual void do_notify();

    bool dirty_;
};

/**************************************************/

/*!
\ingroup depgraph
\brief A output node in Ramen's dependency graph.
*/
class RAMEN_API output_node_t : public node_t
{
public:

    output_node_t();

    /// Signal emitted when this dependency node has changed.
    boost::signals2::signal<void()> changed;

protected:

    output_node_t( const output_node_t& other);

private:

    virtual void do_notify();
};

} // namespace
} // namespace

#endif
