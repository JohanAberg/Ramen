// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_NODES_PLUG_HPP
#define RAMEN_NODES_PLUG_HPP

#include<utility>
#include<vector>
#include<algorithm>

#include<boost/noncopyable.hpp>

#include<ramen/nodes/node_fwd.hpp>

namespace ramen
{

class input_plug_t
{
public:

    input_plug_t( bool optional = false) : optional_( optional), node_(0) {}
    input_plug_t( const input_plug_t& other) : optional_( other.optional_), node_( 0) {}

    bool optional() const { return optional_;}

    bool connected() const	{ return node_ != 0;}
    node_t *input() const	{ return node_;}

    void set_input( node_t *n)  { node_ = n;}
    void clear_input()          { node_ = 0;}

private:

    node_t *node_;
    bool optional_;
};

class output_plug_t : boost::noncopyable
{
public:

    typedef std::pair<node_t*,int> value_type;

    output_plug_t();
    output_plug_t( const output_plug_t& other);

    void add_output( node_t *n, int port);
    void remove_output( node_t *n, int port);

    bool empty() const	{ return nodes_.empty();}

    const value_type& operator[]( int ix) const;

    const std::vector<value_type>& nodes() const    { return nodes_;}
    std::vector<value_type>& nodes()                { return nodes_;}

private:

    std::vector<value_type> nodes_;
};

} // namespace

#endif
