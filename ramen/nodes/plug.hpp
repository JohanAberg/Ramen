// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_NODES_PLUG_HPP
#define RAMEN_NODES_PLUG_HPP

#include<utility>
#include<vector>
#include<algorithm>

#include<boost/tuple/tuple.hpp>

#include<OpenEXR/ImathColor.h>

#include<ramen/assert.hpp>

#include<ramen/dependency/node.hpp>

#include<ramen/nodes/node_fwd.hpp>

#include<ramen/name.hpp>

namespace ramen
{
namespace nodes
{

/*!
\ingroup nodes
\brief Base class for node plugs.
*/
class plug_t
{
protected:

    /// Constructor.
    plug_t( const std::string& id, const Imath::Color3c& color, const std::string& tooltip)
    {
        id_ = name_t( id.c_str());
        color_ = color;
        tooltip_ = name_t( tooltip.c_str());
    }

    /// Copy constructor.
    plug_t( const plug_t& other) : tooltip_( other.tooltip_), color_( other.color_), id_( other.id_) {}

public:

    /// Returns this plug id.
    const name_t& id() const { return id_;}

    /// Returns this plug color. Used in the UI.
    const Imath::Color3c& color() const	{ return color_;}

    /// Returns this plug tooltip. Used in the UI.
    const name_t& tooltip() const	{ return tooltip_;}

    /// Operator less, for assoc. containers (future).
    bool operator<( const plug_t& other) const
    {
        // compare pointers directly.
        return id().c_str() < other.id().c_str();
    }

private:

    name_t id_;
    Imath::Color3c color_;
    name_t tooltip_;
};

/*!
\ingroup nodes
\brief An input plug to which other node plugs can be connected.
*/
class input_plug_t : public plug_t, public dependency::node_t
{
public:

    typedef std::pair<ramen::nodes::node_t*,name_t> connection_t;

    /// Constructor.
    input_plug_t( const std::string& id, bool optional,
                       const Imath::Color3c& color, const std::string& tooltip) : plug_t( id, color, tooltip)
    {
        input_.first = 0;
        optional_ = optional;
    }

    /// Copy constructor.
    input_plug_t( const input_plug_t& other) : plug_t( other)
    {
        input_.first = 0;
        optional_ = other.optional();
    }

    /// Returns if this plug is optional.
    bool optional() const { return optional_;}

    /// Returns true if there's a node connected to this plug.
    bool connected() const	{ return input_.first != 0;}

    /// Returns the node connected to this plug, or null.
    const ramen::nodes::node_t *input_node() const { return input_.first;}

    /// Returns the node connected to this plug, or null.
    ramen::nodes::node_t *input_node() { return input_.first;}

    /// Returns the output plug id of the input node this plug is connected to.
    const name_t& input_node_out_plug() const { return input_.second;}

    /// Sets the node and plug this plug is connected to.
    void set_input( ramen::nodes::node_t *n)
    {
        input_.first = n;
        input_.second = name_t( "unused");
    }

    /// Sets the node and plug this plug is connected to.
    void set_input( ramen::nodes::node_t *n, const name_t& plug)
    {
        input_.first = n;
        input_.second = plug;
    }

    /// Clear this plug connection.
    void clear_input() { input_.first = 0;}

protected:

    void operator=( const input_plug_t& other);

private:

    connection_t input_;
    bool optional_;
};

RAMEN_API input_plug_t *new_clone( const input_plug_t& other);

/*!
\ingroup nodes
\brief An output plug to which other node plugs can be connected.
*/
class output_plug_t : public plug_t, public dependency::output_node_t
{
public:

    typedef boost::tuples::tuple<ramen::nodes::node_t*,name_t, int> connection_t;

    /// Constructor.
    output_plug_t( ramen::nodes::node_t *parent, const std::string& id,
                    const Imath::Color3c& color, const std::string& tooltip);

    virtual ~output_plug_t();

    output_plug_t *clone() const { return do_clone();}

    const ramen::nodes::node_t *parent_node() const { return parent_;}
    ramen::nodes::node_t *parent_node()             { return parent_;}

    void set_parent_node( ramen::nodes::node_t *parent)
    {
        RAMEN_ASSERT( parent);

        parent_ = parent;
    }

    /// Adds a connection to this plug.
    void add_output( ramen::nodes::node_t *n, const name_t& plug);

    /// Adds a connection to this plug.
    void add_output( ramen::nodes::node_t *n, int port);

    /// Removes a connection to this plug.
    void remove_output( ramen::nodes::node_t *n, const name_t& plug);

    /// Removes a connection to this plug.
    void remove_output( ramen::nodes::node_t *n, int port);

    typedef std::vector<connection_t >::const_iterator  const_iterator;
    typedef std::vector<connection_t >::iterator        iterator;

    /// Returns a vector of connections from this plug.
    const std::vector<connection_t >& connections() const { return connections_;}

    /// Returns a vector of connections from this plug.
    std::vector<connection_t >& connections() { return connections_;}

protected:

    output_plug_t( const output_plug_t& other);
    void operator=( const output_plug_t& other);

private:

    output_plug_t *do_clone() const;

    ramen::nodes::node_t *parent_;
    std::vector<connection_t > connections_;
};

RAMEN_API output_plug_t *new_clone( const output_plug_t& other);

} // nodes
} // ramen

#endif

