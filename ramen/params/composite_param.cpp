// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/params/composite_param.hpp>

#include<string>
#include<stdexcept>

#include<boost/bind.hpp>
#include<boost/foreach.hpp>
#include<boost/range/algorithm/for_each.hpp>

#include<ramen/params/param_set.hpp>

#include<ramen/anim/track.hpp>

namespace ramen
{
namespace params
{

composite_param_t::composite_param_t() : param_t(), create_track_( true) {}
composite_param_t::composite_param_t( const std::string& name) : param_t( name), create_track_( true) {}
composite_param_t::composite_param_t( const composite_param_t& other) : param_t( other), params_( other.params_)
{
    create_track_ = other.create_track_;
}

void composite_param_t::do_init()
{
    boost::range::for_each( params(), boost::bind( &param_t::init, _1));
}

void composite_param_t::do_set_param_set( param_set_t *parent)
{
    BOOST_FOREACH( param_t& p, params())
        p.set_param_set( parent);
}

void composite_param_t::do_add_param( param_t *p)
{
    p->set_param_set( param_set());

    if( param_set())
    {
        try
        {
            param_set()->find( p->id());
            throw std::runtime_error( std::string( "Duplicated param id found: ").append( p->id()));
        }
        catch( std::runtime_error& e) {}
    }
    else
    {
        if( find( p->id()))
            throw std::runtime_error( std::string( "Duplicated param id found: ").append( p->id()));
    }

    params().push_back( p);
}

const param_t *composite_param_t::find( const base::name_t& id) const
{
    composite_param_t& self = const_cast<composite_param_t&>( *this);
    return self.find( id);
}

param_t *composite_param_t::find( const base::name_t& id)
{
    BOOST_FOREACH( param_t& p, params())
    {
        if( p.id() == id)
            return &p;

        if( composite_param_t *cp = dynamic_cast<composite_param_t*>( &p))
        {
            param_t *q = cp->find( id);

            if( q)
                return q;
        }
    }

    return 0;
}

void composite_param_t::do_set_frame( float frame)
{
    boost::range::for_each( params(), boost::bind( &param_t::set_frame, _1, frame));
}

void composite_param_t::do_create_tracks( anim::track_t *parent)
{
    if( create_track_)
    {
        std::auto_ptr<anim::track_t> t( new anim::track_t( name()));
        boost::range::for_each( params(), boost::bind( &param_t::create_tracks, _1, t.get()));

        if( t->num_children() != 0)
            parent->add_child( t);
    }
    else
        boost::range::for_each( params(), boost::bind( &param_t::create_tracks, _1, parent));
}

void composite_param_t::do_evaluate( float frame)
{
    boost::range::for_each( params(), boost::bind( &param_t::evaluate, _1, frame));
}

void composite_param_t::do_add_to_hash( hash::generator_t& hash_gen) const
{
    BOOST_FOREACH( const param_t& p, params())
        p.add_to_hash( hash_gen);
}

void composite_param_t::do_convert_relative_paths( const boost::filesystem::path& old_base, const boost::filesystem::path& new_base)
{
    boost::range::for_each( params_, boost::bind( &param_t::convert_relative_paths, _1, old_base, new_base));
}

void composite_param_t::do_make_paths_absolute()
{
    boost::range::for_each( params_, boost::bind( &param_t::make_paths_absolute, _1));
}

void composite_param_t::do_make_paths_relative()
{
    boost::range::for_each( params(), boost::bind( &param_t::make_paths_relative, _1));
}

// util
void composite_param_t::do_apply_function( const boost::function<void ( param_t*)>& f)
{
    boost::range::for_each( params(), boost::bind( &param_t::apply_function, _1, f));
}

void composite_param_t::do_read( serialization::yaml_iarchive_t& node)
{
    RAMEN_ASSERT( param_set());

    serialization::yaml_node_t nodes = node.get_node( "children");

    for( int i = 0; i < nodes.size(); ++i)
        param_set()->read_param( nodes[i]);
}

void composite_param_t::do_write( serialization::yaml_oarchive_t& out) const
{
    out << YAML::Key << "children" << YAML::Value;
        out.begin_seq();
            boost::range::for_each( params(), boost::bind( &param_t::write, _1, boost::ref( out)));
        out.end_seq();
}

} // namespace
} // namespace
