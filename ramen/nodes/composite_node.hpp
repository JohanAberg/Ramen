// Copyright (c) 2012 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_NODES_COMPOSITE_NODE_HPP
#define RAMEN_NODES_COMPOSITE_NODE_HPP

#include<ramen/nodes/node.hpp>

#include<memory>
#include<set>

#include<boost/tokenizer.hpp>

#include<ramen/nodes/graph.hpp>
#include<ramen/nodes/visitor.hpp>

#include<ramen/serialization/archive_fwd.hpp>
#include<ramen/serialization/yaml.hpp>

#include<ramen/ui/graph_layout_fwd.hpp>

namespace ramen
{
namespace nodes
{

/**
\ingroup nodes
\brief Abstract base class for nodes that contain other nodes.
*/
class composite_node_t : public node_t
{
public:

    /// Constructor.
    composite_node_t();

    /// Destructor.
    virtual ~composite_node_t();

    /// Called for the new node, after being copied.
    virtual void cloned();

    /// Dispatch function for visitor pattern.
    virtual void accept( visitor_t& v);

    /// Returns a const reference to the node graph inside this node.
    const graph_t& graph() const { return g_;}

    /// Returns a reference to the node graph inside this node.
    graph_t& graph() { return g_;}

    // iterators & ranges

    typedef graph_t::const_node_iterator    const_iterator;
    typedef graph_t::node_iterator          iterator;

    typedef graph_t::node_range_type        node_range_type;
    typedef graph_t::const_node_range_type  const_node_range_type;

    /// Returns a const range to the child nodes.
    const_node_range_type nodes() const { return g_.nodes();}

    /// Returns a range to the child nodes.
    node_range_type nodes() { return g_.nodes();}

    /// Creates a new node as a child of this node.
    node_t *create_node_by_id( const std::string& id, bool ui = false);

    /// Creates a new node as a child of this node.
    node_t *create_node_by_id_with_version( const std::string& id, const std::pair<int, int>& version);

    /// Adds a node as a child of this node.
    void add_node( std::auto_ptr<node_t> n);

    /// Removes a child node from this node.
    std::auto_ptr<node_t> release_node( node_t *n);

    /// Finds the children node with the given name.
    const node_t *find_node( const std::string& name) const;

    /// Finds the children node with the given name.
    node_t *find_node( const std::string& name);

    // connections
    void connect( node_t *src, const base::name_t& src_plug,
                    node_t *dst, const base::name_t& dst_plug);

    void disconnect( node_t *src, const base::name_t& src_plug,
                        node_t *dst, const base::name_t& dst_plug);

    // layouts

    /// Returns a const reference to this node graph layout.
    const ui::graph_layout_t& layout() const { return *layout_;}

    /// Returns a reference to this node graph layout.
    ui::graph_layout_t& layout() { return *layout_;}

    /// Sets this node graph layout.
    void set_layout( std::auto_ptr<ui::graph_layout_t> layout);

protected:

    typedef boost::tokenizer<boost::char_separator<char> > name_tokenizer_type;

    composite_node_t( const composite_node_t& other);
    void operator=( const composite_node_t&);

    // node factory
    std::auto_ptr<node_t> do_create_node_by_id( const std::string& id, bool ui = false) const;
    std::auto_ptr<node_t> do_create_node_by_id_with_version( const std::string& id, const std::pair<int, int>& version) const;

    node_t *do_find_node( name_tokenizer_type::iterator tok_it, name_tokenizer_type::iterator tok_end);

    void for_each_sub_graph( const boost::function<void( sub_graph_t*)> *f);

private:

    std::auto_ptr<node_t> create_unknown_node( const std::string& id, const std::pair<int, int>& version) const;

    graph_t g_;
    std::auto_ptr<ui::graph_layout_t> layout_;
};

} // namespace
} // namespace

#endif
