// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/params/param.hpp>

#include<stdexcept>

#include<ramen/params/parameterised.hpp>

#include<ramen/dependency/graph.hpp>

#include<ramen/util/string.hpp>
#include<ramen/util/flags.hpp>

namespace ramen
{
namespace params
{

param_t::param_t() : param_set_( 0)
{
    flags_ = persist_bit | can_undo_bit | enabled_bit | include_in_hash_bit;
}

param_t::param_t( const std::string& name) : param_set_( 0), name_( name)
{
    flags_ = persist_bit | can_undo_bit | enabled_bit | include_in_hash_bit;
}

param_t::param_t( const param_t& other) : param_set_( 0), id_( other.id_), name_( other.name_),
                                            value_( other.value_), flags_( other.flags_)
{
}

void param_t::init() { do_init();}

void param_t::do_init() {}

void param_t::set_id( const base::name_t& identifier)
{
    RAMEN_ASSERT( util::is_string_valid_identifier( identifier.string()));

    if( !util::is_string_valid_identifier( identifier.string()))
        throw std::runtime_error( "Invalid id for param_t");

    id_ = identifier;
}

void param_t::set_param_set( param_set_t *parent)
{
    param_set_ = parent;
    do_set_param_set( parent);
}

void param_t::do_set_param_set( param_set_t *parent) {}

const parameterised_t *param_t::parameterised() const
{
    RAMEN_ASSERT( param_set());
    return param_set()->parent();
}

parameterised_t *param_t::parameterised()
{
    RAMEN_ASSERT( param_set());
    return param_set()->parent();
}

const nodes::world_node_t *param_t::world() const
{
    RAMEN_ASSERT( param_set());
    const parameterised_t *p = parameterised();

    if( p)
        return p->world();
    else
        return 0;
}

nodes::world_node_t *param_t::world()
{
    RAMEN_ASSERT( param_set());
    parameterised_t *p = parameterised();

    if( p)
        return p->world();
    else
        return 0;
}

bool param_t::enabled() const { return util::test_flag( flags_, enabled_bit);}

void param_t::set_enabled( bool e)
{
    util::set_flag( flags_, enabled_bit, e);
}

bool param_t::is_static() const      { return util::test_flag( flags_, static_bit);}

void param_t::set_static( bool b)
{
    util::set_flag( flags_, static_bit, b);
}

bool param_t::secret() const		{ return util::test_flag( flags_, secret_bit);}

void param_t::set_secret( bool b)
{
    util::set_flag( flags_, secret_bit, b);
}

bool param_t::persist() const	{ return util::test_flag( flags_, persist_bit);}

void param_t::set_persist( bool b)
{
    util::set_flag( flags_, persist_bit, b);
}

bool param_t::can_undo() const	{ return util::test_flag( flags_, can_undo_bit);}

void param_t::set_can_undo( bool b)
{
    util::set_flag( flags_, can_undo_bit, b);
}

bool param_t::include_in_hash() const { return util::test_flag( flags_, include_in_hash_bit);}

void param_t::set_include_in_hash( bool b)
{
    util::set_flag( flags_, include_in_hash_bit, b);
}

bool param_t::can_have_expressions() const	{ return util::test_flag( flags_, can_have_expressions_bit);}

void param_t::set_can_have_expressions( bool b)
{
    util::set_flag( flags_, can_have_expressions_bit, b);
}

bool param_t::track_mouse() const
{
    if( const parameterised_t *p = parameterised())
    {
        if( !p->track_mouse())
            return false;
    }

    return !( flags_ & dont_track_mouse_bit);
}

void param_t::set_track_mouse( bool b)
{
    util::set_flag( flags_, dont_track_mouse_bit, b);
}

void param_t::emit_param_changed( change_reason reason)
{
    if( reason != silent_edit)
    {
        if( param_set_t *p = param_set())
            p->param_changed( this, reason);
    }
}

// util
void param_t::apply_function( const boost::function<void ( param_t*)> *f)
{
    (*f)( this);
    do_apply_function( f);
}

void param_t::do_apply_function( const boost::function<void ( param_t*)> *f) {}

// python interop
boost::python::object param_t::to_python( const base::poly_regular_t& v) const
{
    PyErr_SetString( PyExc_TypeError, "Can't convert value from C++");
    boost::python::throw_error_already_set();
    return boost::python::object();
}

base::poly_regular_t param_t::from_python( const boost::python::object& obj) const
{
    throw std::bad_cast();
    return base::poly_regular_t();
}

param_t *new_clone( const param_t& other) { return other.clone();}

} // namespace
} // namespace
