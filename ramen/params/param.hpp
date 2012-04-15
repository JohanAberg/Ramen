// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_PARAM_HPP
#define RAMEN_PARAM_HPP

#include<ramen/config.hpp>

#include<ramen/python/python.hpp>

#include<memory>
#include<sstream>
#include<string>
#include<vector>
#include<utility>

#include<boost/flyweight.hpp>

#include<OpenEXR/ImathBox.h>

#include<QObject>

#include<ramen/assert.hpp>

#include<ramen/params/poly_param_value.hpp>

#include<ramen/params/parameterised_fwd.hpp>
#include<ramen/params/param_set_fwd.hpp>

#include<ramen/params/static_param_command_fwd.hpp>
#include<ramen/params/animated_param_command_fwd.hpp>

#include<boost/filesystem/fstream.hpp>

#include<ramen/app/composition_fwd.hpp>

#include<ramen/anim/track_fwd.hpp>

#include<ramen/expressions/expression.hpp>

#include<ramen/undo/command.hpp>

#include<ramen/util/hash_generator.hpp>

#include<ramen/serialization/yaml_iarchive.hpp>
#include<ramen/serialization/yaml_oarchive.hpp>

#include<boost/python/object.hpp>
#include<boost/python/extract.hpp>
#include<ramen/python/access_fwd.hpp>

#ifndef RAMEN_NO_GUI
	#include<ramen/ui/widgets/param_spinbox_fwd.hpp>
#endif

class QWidget;

namespace ramen
{

/**
\ingroup params
\brief param class
*/
class RAMEN_API param_t : public QObject
{
    Q_OBJECT

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
		can_have_expressions_bit	= 1 << 9
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
	
    param_t();
    explicit param_t( const std::string& name);
    virtual ~param_t() {}

    virtual void init() {}

    param_t *clone() const { return do_clone();}

    const std::string& name() const	    { return name_;}
    void set_name( const std::string& name) { name_ = name;}

    const std::string& id() const		{ return id_;}
    void set_id( const std::string& identifier);

    const param_set_t *param_set() const    { return param_set_;}
    param_set_t *param_set()				{ return param_set_;}

    virtual void set_param_set( param_set_t *parent);

    const parameterised_t *parameterised() const;
    parameterised_t *parameterised();

    const composition_t *composition() const;
    composition_t *composition();

    bool enabled() const { return flags_ & enabled_bit;}
    void set_enabled( bool e);

    bool is_static() const      { return flags_ & static_bit;}
    void set_static( bool b);

    bool secret() const		{ return flags_ & secret_bit;}
    void set_secret( bool b);

    bool persist() const	{ return flags_ & persist_bit;}
    void set_persist( bool b);

    bool can_undo() const	{ return flags_ & can_undo_bit;}
    void set_can_undo( bool b);

	bool include_in_hash() const { return flags_ &  include_in_hash_bit;}
	void set_include_in_hash( bool b);			

    bool can_have_expressions() const	{ return flags_ & can_have_expressions_bit;}
    void set_can_have_expressions( bool b);
	
    bool track_mouse() const;
    void set_track_mouse( bool b);
	
	// toolip
	const std::string& tooltip() const		{ return tooltip_;}
	void set_tooltip( const std::string& t) { tooltip_ = t;}
	
	// notifications
	void emit_param_changed( change_reason reason);
	
	// formats
    void format_changed( const Imath::Box2i& new_format, float aspect, const Imath::V2f& proxy_scale)
	{ 
		do_format_changed( new_format, aspect, proxy_scale);
	}
	
    // animation
    void create_tracks( anim::track_t *parent);
	virtual void set_frame( float frame);
    void evaluate( float frame);

	int num_expressions() const;
	const expressions::expression_t& expression( int indx = 0) const;	
	expressions::expression_t& expression( int indx = 0);	
	
    // hash
	void add_to_hash( hash_generator_t& hash_gen) const;
    
    // undo
    virtual std::auto_ptr<undo::command_t> create_command() { return std::auto_ptr<undo::command_t>();}

    // paths
    virtual void convert_relative_paths( const boost::filesystem::path& old_base, const boost::filesystem::path& new_base);
	virtual void make_paths_absolute();
	virtual void make_paths_relative();
	
    // serialization
	virtual void read( const serialization::yaml_node_t& node);
    virtual void write( serialization::yaml_oarchive_t& out) const;

	// util
	virtual void apply_function( const boost::function<void ( param_t*)>& f);
	
    // widgets
	#ifndef RAMEN_NO_GUI	
	    QWidget *create_widgets();
	#endif
		
    void update_widgets()           { do_update_widgets();}
    void enable_widgets( bool e)    { do_enable_widgets( e);}
	
protected:

    param_t( const param_t& other);
    void operator=( const param_t& other);

    int flags() const			{ return flags_;}
    void set_flags( int flags)	{ flags_ = flags;}

    const poly_param_value_t& value() const	{ return value_;}
    poly_param_value_t& value()             { return value_;}

    virtual poly_param_value_t value_at_frame( float frame) const { return value();}

	// expressions
	void add_expression( const std::string& name);
	bool eval_expression( int index, float frame, float& v, ui::param_spinbox_t *widget) const;
	void read_expressions( const serialization::yaml_node_t& node);
	void write_expressions( serialization::yaml_oarchive_t& out) const;
	
private:

    friend class param_set_t;
    friend class static_param_command_t;
    friend class static_param_reset_command_t;
    friend class animated_param_command_t;
    friend class animated_param_reset_command_t;
		
    virtual param_t *do_clone() const = 0;
	
    virtual void do_format_changed( const Imath::Box2i& new_format, float aspect, const Imath::V2f& proxy_scale) {}

	// time and anim
    virtual void do_create_tracks( anim::track_t *parent) {}
    virtual void do_evaluate( float frame);
	expressions::expression_t *find_expression( const std::string& name);
	
	// hash
	virtual void do_add_to_hash( hash_generator_t& hash_gen) const;

	// python interop
	friend class python::access;
	
	virtual boost::python::object to_python( const poly_param_value_t& v) const;
	virtual poly_param_value_t from_python( const boost::python::object& obj) const;

	// serialization
	virtual void do_read( const serialization::yaml_node_t& node);
    virtual void do_write( serialization::yaml_oarchive_t& out) const;
	
	// widgets
	#ifndef RAMEN_NO_GUI
	    virtual QWidget *do_create_widgets()        { return 0;}
	#endif
		
    virtual void do_update_widgets()            {}
    virtual void do_enable_widgets( bool e)     {}

    template<class S> friend S get_value( const param_t& p);
    template<class S> friend S get_value_at_frame( const param_t& p, float frame);

    template<class S> friend S get_absolute_value( const param_t& p);
    template<class S> friend S get_absolute_value_at_frame( const param_t& p, float frame);

    param_set_t *param_set_;

    boost::flyweight<std::string> name_;
    boost::flyweight<std::string> id_;
    boost::flyweight<std::string> tooltip_;
	
    boost::uint32_t flags_;
    poly_param_value_t value_;
	
    std::vector<std::pair<boost::flyweight<std::string>, expressions::expression_t> > expressions_;	
};

template<class S>
S get_value( const param_t& p)
{ 
    const poly_param_value_t& any( p.value());

	#ifdef NDEBUG
	    return any.cast<S>();
	#else
		try
		{
			return any.cast<S>();
		}
		catch( adobe::bad_cast& e)
		{
			RAMEN_ASSERT( 0 && "Bad cast exception in get_value");
		}
	#endif
}

template<class S>
S get_value_at_frame( const param_t& p, float frame)
{
    poly_param_value_t any( p.value_at_frame( frame));

	#ifdef NDEBUG
		return any.cast<S>();
	#else
		try
		{
			return any.cast<S>();
		}
		catch( adobe::bad_cast& e)
		{
			RAMEN_ASSERT( 0 && "Bad cast exception in get_value");
		}
	#endif
}

param_t* new_clone( const param_t& other);

} // namespace

#endif
