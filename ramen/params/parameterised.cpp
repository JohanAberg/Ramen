// Copyright (c) 2010 Esteban Tovagliari

#include<ramen/params/parameterised.hpp>

#include<boost/foreach.hpp>
#include<boost/bind.hpp>

#include<adobe/algorithm/for_each.hpp>

#include<ramen/app/composition.hpp>

#include<ramen/anim/track.hpp>

#include<ramen/serialization/yaml_oarchive.hpp>

#include<ramen/util/validate_string.hpp>

namespace ramen
{

parameterised_t::parameterised_t() : dont_persist_params_( false)
{
    params_.set_parent( this);
}

parameterised_t::parameterised_t( const parameterised_t& other) : params_( other.params_)
{
    name_ = other.name_;
    dont_persist_params_ = other.dont_persist_params_;
    params_.set_parent( this);
}

parameterised_t::~parameterised_t() { deleted( this);}

void parameterised_t::create_params()
{
    do_create_params();
    adobe::for_each( param_set(), boost::bind( &param_t::init, _1));
}

const node_t *parameterised_t::node() const { return dynamic_cast<const node_t*>( this);}
node_t *parameterised_t::node()				{ return dynamic_cast<node_t*>( this);}

// composition
const composition_t *parameterised_t::composition() const
{
	const node_t *n = node();
	
	if( n)
		return n->composition();
	
	return 0;
}

composition_t *parameterised_t::composition()
{
	node_t *n = node();
	
	if( n)
		return n->composition();
	
	return 0;
}

void parameterised_t::set_name( const std::string& n)
{
	RAMEN_ASSERT( is_string_valid_identifier( n));
	
	if( !is_string_valid_identifier( n))
		throw std::runtime_error( "Invalid name for parameterised_t");
	
	name_ = n;
}

bool parameterised_t::autokey() const
{
    if( const composition_t *c = composition())
        return c->autokey();

    return false;
}

bool parameterised_t::track_mouse() const { return true;}

void parameterised_t::do_create_params() {}

const param_t& parameterised_t::param( const std::string& identifier) const
{
    return param_set().find( identifier);
}

param_t& parameterised_t::param( const std::string& identifier)
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

void parameterised_t::do_create_tracks( anim::track_t *parent) {}

void parameterised_t::set_frame( float f)
{
    adobe::for_each( param_set(), boost::bind( &param_t::set_frame, _1, f));
    do_set_frame( f);
}

void parameterised_t::evaluate_params( float frame)
{
    adobe::for_each( param_set(), boost::bind( &param_t::evaluate, _1, frame));
}

void parameterised_t::update_widgets()
{
    adobe::for_each( param_set(), boost::bind( &param_t::update_widgets, _1));
	do_update_widgets();
}

void parameterised_t::do_update_widgets() {}

} // namespace
