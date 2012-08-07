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

// util
void composite_param_t::do_apply_function( const boost::function<void ( param_t*)> *f)
{
    boost::range::for_each( params(), boost::bind( &param_t::apply_function, _1, f));
}

} // namespace
} // namespace
