// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/params/parameterised.hpp>

#include<boost/foreach.hpp>
#include<boost/bind.hpp>
#include<boost/range/algorithm/for_each.hpp>

#include<ramen/nodes/world_node.hpp>

#include<ramen/anim/track.hpp>

#include<ramen/serialization/yaml_oarchive.hpp>

#include<ramen/util/string.hpp>

namespace ramen
{
namespace params
{

parameterised_t::parameterised_t() : dont_persist_params_( false)
{
    params_.set_parent( this);
    parent_ = 0;
}

parameterised_t::parameterised_t( const parameterised_t& other) : params_( other.params_)
{
    name_ = other.name_;
    dont_persist_params_ = other.dont_persist_params_;
    params_.set_parent( this);
    parent_ = 0;
}

parameterised_t::~parameterised_t() { deleted( this);}

void parameterised_t::init()
{
    create_params();
    create_manipulators();
    do_init();
}

void parameterised_t::do_init() {}

parameterised_t *parameterised_t::clone() const { return do_clone();}

void parameterised_t::create_params()
{
    RAMEN_ASSERT( world());

    do_create_params();
    boost::range::for_each( param_set(), boost::bind( &param_t::init, _1));
}

void parameterised_t::set_parent( parameterised_t *parent)
{
    parent_ = parent;
    do_set_parent( parent);
}

void parameterised_t::do_set_parent( parameterised_t *parent) {}

const nodes::node_t *parameterised_t::node() const
{
    const parameterised_t *p = this;

    while( p != 0)
    {
        if( const nodes::node_t *node = dynamic_cast<const nodes::node_t*>( p))
            return node;

        p = p->parent();
    }

    return 0;
}

nodes::node_t *parameterised_t::node()
{
    parameterised_t *p = this;

    while( p != 0)
    {
        if( nodes::node_t *node = dynamic_cast<nodes::node_t*>( p))
            return node;

        p = p->parent();
    }

    return 0;
}

const nodes::world_node_t *parameterised_t::world() const
{
    const parameterised_t *p = this;

    while( p != 0)
    {
        if( const nodes::world_node_t *world = dynamic_cast<const nodes::world_node_t*>( p))
            return world;

        p = p->parent();
    }

    return 0;
}

nodes::world_node_t *parameterised_t::world()
{
    parameterised_t *p = this;

    while( p != 0)
    {
        if( nodes::world_node_t *world = dynamic_cast<nodes::world_node_t*>( p))
            return world;

        p = p->parent();
    }

    return 0;
}

void parameterised_t::set_name( const std::string& n)
{
    RAMEN_ASSERT( util::is_string_valid_identifier( n));

    if( !util::is_string_valid_identifier( n))
        throw std::runtime_error( "Invalid name for parameterised_t");

    name_ = n;
}

bool parameterised_t::autokey() const
{
    //if( const composition_t *c = composition())
    //    return c->autokey();

    return false;
}

bool parameterised_t::track_mouse() const { return true;}

const param_t& parameterised_t::param( const base::name_t& identifier) const
{
    return param_set().find( identifier);
}

param_t& parameterised_t::param( const base::name_t& identifier)
{
    return param_set().find( identifier);
}

void parameterised_t::for_each_param( const boost::function<void ( param_t*)>& f)
{
    param_set().for_each_param( f);
}

// anim & time
void parameterised_t::create_tracks( anim::track_t *root)
{
    std::auto_ptr<anim::track_t> top( new anim::track_t( name()));

    BOOST_FOREACH( param_t& p, param_set())
    {
        p.create_tracks( top.get());
    }

    do_create_tracks( top.get());
    root->add_child( top);
}

void parameterised_t::set_frame( float f)
{
    boost::range::for_each( param_set(), boost::bind( &param_t::set_frame, _1, f));
    do_set_frame( f);
}

void parameterised_t::evaluate_params( float frame)
{
    boost::range::for_each( param_set(), boost::bind( &param_t::evaluate, _1, frame));
}

parameterised_t *new_clone( const parameterised_t& other) { return other.clone();}

} // namespace
} // namespace
