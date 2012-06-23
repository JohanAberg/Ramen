// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/params/inspector_param.hpp>

#include<boost/bind.hpp>

#include<QFrame>

#include<ramen/app/application.hpp>

#include<ramen/params/parameterised.hpp>

#include<ramen/ui/user_interface.hpp>
#include<ramen/ui/inspector/inspector.hpp>
#include<ramen/ui/inspector/panel.hpp>

namespace ramen
{
namespace params
{

inspector_param_t::inspector_param_t( const name_t& id) : param_t()
{
    set_static( true);
    set_id( id);
    contents_ = 0;
}

inspector_param_t::inspector_param_t( const inspector_param_t& other) : param_t( other), contents_( 0)
{
}

inspector_param_t::~inspector_param_t() { connection_.disconnect();}

void inspector_param_t::set_parameterised( parameterised_t *p)
{
    if( contents_)
    {
        connection_.disconnect();
        contents_ = 0;
    }

    if( p)
    {
        contents_ = p;
        connection_ = p->deleted.connect( boost::bind( &inspector_param_t::parameterised_deleted, this, _1));
    }
}

void inspector_param_t::parameterised_deleted( parameterised_t *p)
{
    if( p == contents_)
        set_parameterised( 0);
}

} // namespace
} // namespace
