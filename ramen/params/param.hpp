// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_PARAM_HPP
#define RAMEN_PARAM_HPP

#include<ramen/config.hpp>

#include<ramen/params/param_fwd.hpp>
#include<ramen/python/python.hpp>
#include<ramen/dependency/node.hpp>

#include<memory>
#include<sstream>
#include<string>
#include<vector>
#include<utility>

#include<boost/python/object.hpp>
#include<boost/python/extract.hpp>

#include<OpenEXR/ImathBox.h>

#include<base/name.hpp>

#include<ramen/assert.hpp>
#include<ramen/nodes/node_fwd.hpp>
#include<ramen/poly_indexable_regular.hpp>
#include<ramen/params/parameterised_fwd.hpp>
#include<ramen/params/param_set_fwd.hpp>

namespace ramen
{
namespace params
{

/**
\ingroup params
\brief param class
*/
class RAMEN_API param_t : public dependency::node_t
{
protected:

    enum flag_bits
    {
        static_bit					= 1 << 0,   // param does not animate.
        secret_bit					= 1 << 1,   // param does not show on the inspector.
        persist_bit					= 1 << 2,   // param value is not saved to file.
        can_undo_bit				= 1 << 3,   // param uses undo.
        enabled_bit					= 1 << 4,   // param widgets are enabled in the inspector.
        dont_track_mouse_bit		= 1 << 5,   // notify is called on mouse up only.
        round_to_int_bit			= 1 << 6,   // for float params, round to value to integer. Emulates an integer_param.
        proportional_bit			= 1 << 7,   // param is a float2 or float3 and the values can be modified proportionally.
        include_in_hash_bit			= 1 << 8,	// param is included in hash
        can_have_expressions_bit	= 1 << 9    // param can have expressions
    };

public:

    enum change_reason
    {
        silent_edit = 0,
        user_edited,
        node_edited,
        node_loaded,
        time_changed
    };

    /// Constructor.
    param_t();

    /// Constructor.
    explicit param_t( const std::string& name);

    /// Destructor.
    virtual ~param_t() {}

    /// Post constructor initialization.
    void init();

    /// Makes a copy of this param.
    param_t *clone() const { return do_clone();}

    /// Returns the param name.
    const base::name_t& name() const { return name_;}

    /// Sets the param name.
    void set_name( const base::name_t& name) { name_ = name;}

    /// Returns the param id.
    const base::name_t& id() const { return id_;}

    /// Sets the param id.
    void set_id( const base::name_t& identifier);

    /// Returns a const pointer to the param set this param belongs to.
    const param_set_t *param_set() const    { return param_set_;}

    /// Returns a pointer to the param set this param belongs to.
    param_set_t *param_set()				{ return param_set_;}

    /// Sets the param set this param belongs to.
    void set_param_set( param_set_t *parent);

    /// Returns a const pointer to the parameterised this param belongs to.
    const parameterised_t *parameterised() const;

    /// Returns a pointer to the parameterised this param belongs to.
    parameterised_t *parameterised();

    /// Returns the world node this param belongs to.
    const nodes::world_node_t *world() const;

    /// Returns the world node this param belongs to.
    nodes::world_node_t *world();

    // flags
    bool enabled() const;
    void set_enabled( bool e);

    bool is_static() const;
    void set_static( bool b);

    bool secret() const;
    void set_secret( bool b);

    bool persist() const;
    void set_persist( bool b);

    bool can_undo() const;
    void set_can_undo( bool b);

    bool include_in_hash() const;
    void set_include_in_hash( bool b);

    bool can_have_expressions() const;
    void set_can_have_expressions( bool b);

    bool track_mouse() const;
    void set_track_mouse( bool b);

    // toolips
    const std::string& tooltip() const		{ return tooltip_;}
    void set_tooltip( const std::string& t) { tooltip_ = t;}

    // notifications
    void emit_param_changed( change_reason reason);

    // util
    void apply_function( const boost::function<void ( param_t*)> *f);

protected:

    param_t( const param_t& other);
    void operator=( const param_t& other);

    int flags() const			{ return flags_;}
    void set_flags( int flags)	{ flags_ = flags;}

    const base::poly_regular_t& value() const	{ return value_;}
    base::poly_regular_t& value()               { return value_;}

private:

    friend class param_set_t;

    virtual param_t *do_clone() const = 0;

    virtual void do_init();

    virtual void do_set_param_set( param_set_t *parent);

    // python
    virtual boost::python::object to_python( const base::poly_regular_t& v) const;
    virtual base::poly_regular_t from_python( const boost::python::object& obj) const;

    // util
    virtual void do_apply_function( const boost::function<void ( param_t*)> *f);

    param_set_t *param_set_;

    base::name_t name_;
    base::name_t id_;
    std::string tooltip_;

    boost::uint32_t flags_;
    base::poly_regular_t value_;
};

RAMEN_API param_t *new_clone( const param_t& other);

} // namespace
} // namespace

#endif
