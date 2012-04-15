// Copyright (c) 2010 Esteban Tovagliari

#include<ramen/nodes/node.hpp>

#include<vector>
#include<stdexcept>
#include<algorithm>

#include<boost/bind.hpp>
#include<boost/foreach.hpp>

#include<adobe/algorithm/for_each.hpp>

#include<ramen/nodes/graph_algorithm.hpp>

#include<ramen/app/composition.hpp>

#include<ramen/serialization/yaml_iarchive.hpp>
#include<ramen/serialization/yaml_oarchive.hpp>

#include<ramen/util/validate_string.hpp>

namespace ramen
{
namespace
{
	
struct frames_needed_less
{
	bool operator()( const std::pair<int, int>& a, const std::pair<int, int>& b) const
	{
		if( a.first < b.first)
			return true;
		
		if( a.first == b.first)
			return a.second < b.second;
		
		return false;
	}
};
	
} // unnamed
	
node_t::node_t() : manipulable_t(), flags_( 0), dependency_count_( 0), composition_( 0), output_( 0)
{
}

node_t::node_t( const node_t& other) : manipulable_t( other)
{
    if( other.output_)
        add_output_plug();

	dependency_count_ = 0;
    flags_ = other.flags_;
    loc_ = other.loc_;
    plugs_info_ = other.plugs_info_;
    composition_ = other.composition_;
}

node_t::~node_t()
{
    if( output_ )
        delete output_;
}

node_t *node_t::clone() const
{
    node_t *n = do_clone();

    for( int i = 0; i < n->num_inputs(); ++i)
        n->connected( 0, i);

    n->create_manipulators();
    n->cloned();
    return n;
}

// visitor
void node_t::accept( node_visitor& v) { v.visit( this);}

bool node_t::selected() const { return flags_ & selected_bit;}

void node_t::select( bool state)
{
    if( state)
	{
		if( ui_invisible())
		{
			// ui invisible nodes can't be selected
			flags_ &= ~selected_bit;
		}
		else
			flags_ |= selected_bit;
	}
    else
		flags_ &= ~selected_bit;
}

void node_t::toggle_selection()	{ select( !selected());}

bool node_t::ignored() const	{ return flags_ & ignored_bit;}

void node_t::set_ignored( bool b)
{
    if( b)
		flags_ |= ignored_bit;
    else
		flags_ &= ~ignored_bit;
}

bool node_t::plugin_error() const
{
    return flags_ & plugin_error_bit;
}

void node_t::set_plugin_error( bool b)
{
    if( b)
		flags_ |= plugin_error_bit;
    else
		flags_ &= ~plugin_error_bit;
}

bool node_t::autolayout() const
{
    return flags_ & autolayout_bit;
}

void node_t::set_autolayout( bool b)
{
    if( b)
		flags_ |= autolayout_bit;
    else
		flags_ &= ~autolayout_bit;
}

bool node_t::cacheable() const	{ return flags_ & cacheable_bit;}

void node_t::set_cacheable( bool b)
{
    if( b)
		flags_ |= cacheable_bit;
    else
		flags_ &= ~cacheable_bit;
}

bool node_t::notify_dirty() const { return flags_ & notify_dirty_bit;}

void node_t::set_notify_dirty( bool b)
{
    if( b)
		flags_ |= notify_dirty_bit;
    else
		flags_ &= ~notify_dirty_bit;
}

bool node_t::ui_invisible() const { return flags_ & ui_invisible_bit;}

void node_t::set_ui_invisible( bool b)
{
    if( b)
		flags_ |= ui_invisible_bit;
    else
		flags_ &= ~ui_invisible_bit;
}

bool node_t::is_active() const     { return flags_ & active_bit;}
bool node_t::is_context() const    { return flags_ & context_bit;}

// inputs

void node_t::add_input_plug( const input_plug_info_t& info, bool optional)
{
    plugs_info_.push_back( info);
    inputs_.push_back( input_plug_t( optional));
}

void node_t::set_plug_info( std::size_t index, const input_plug_info_t& info) { plugs_info_[index] = info;}

bool node_t::accept_connection( node_t *src, int port) const { return true;}

void node_t::connected( node_t *src, int port)
{
    if( variable_num_inputs() && src != 0 && port == num_inputs()-1)
		add_new_input_plug();

    do_connected( src, port);
}

void node_t::do_connected( node_t *src, int port) {}

void node_t::add_new_input_plug()
{
    add_input_plug( input_plug_info_t( ui::palette_t::Instance().color("back plug")), true);
    reconnect_node();
}

void node_t::reconnect_node()
{
    composition_t *comp = composition();

    if( comp)
    {
        BOOST_FOREACH( edge_t& e, comp->edges())
        {
            if( e.dst == this)
                input_plugs()[ e.port].set_input( e.src);
        }
    }
}

// params
void node_t::param_edit_finished() { notify();}

void node_t::notify()
{
	if( dependency_count())
	{
		RAMEN_ASSERT( composition());
		set_notify_dirty( true);
		composition()->notify_all_dirty();
	}
	else
		breadth_first_outputs_search( *this, boost::bind( &node_t::do_notify, _1));
}

void node_t::do_notify()
{ 
	RAMEN_ASSERT( composition());

	changed( this);
	set_notify_dirty( false);
}

node_t::frame_interval_type node_t::frame_interval() const { return do_calc_frame_interval();}

void node_t::calc_frames_needed( const render::context_t& context)
{ 
	do_calc_frames_needed( context);
	
	if( !frames_needed().empty())
		std::sort( frames_needed().begin(), frames_needed().end(), frames_needed_less());
}

void node_t::do_calc_frames_needed( const render::context_t& context) {}

// expressions
void node_t::inc_dependency_count() { ++dependency_count_;}

void node_t::dec_dependency_count()
{
	RAMEN_ASSERT( dependency_count_ >= 1);
	--dependency_count_;
}

int node_t::dependency_count() const
{ 
	RAMEN_ASSERT( dependency_count_ >= 0);
	return dependency_count_;
}

void node_t::begin_active()
{
    flags_ |= active_bit;
    do_begin_active();
}

void node_t::end_active()
{
    do_end_active();
    flags_ &= ~active_bit;
}

void node_t::begin_context()
{
    flags_ |= context_bit;
    do_begin_context();
}

void node_t::end_context()
{
    do_end_context();
    flags_ &= ~context_bit;
}

bool node_t::interacting() const
{
	return flags_ & interacting_bit;
}

void node_t::begin_interaction()
{
	do_begin_interaction();
	flags_ |= interacting_bit;
}

void node_t::end_interaction()
{
	do_end_interaction();
	flags_ &= ~interacting_bit;
}

bool node_t::is_valid() const
{
    BOOST_FOREACH( const input_plug_t& i, input_plugs())
    {
        if( !i.connected() && !i.optional())
            return false;
    }

    if( ignored())
    {
        if( num_inputs() == 0)
            return false;

        // handle the all inputs optional case
        bool all_optional = true;
        bool all_disconnected = true;

        BOOST_FOREACH( const input_plug_t& i, input_plugs())
        {
            if( i.connected())
                all_disconnected = false;
                
            if( !i.optional())
                all_optional = false;
        }
        
        if( all_optional && all_disconnected)
            return false;
    }

    return do_is_valid();
}

bool node_t::do_is_valid() const { return true;}

bool node_t::is_identity() const
{
	RAMEN_ASSERT( is_valid());

	if( ignored())
		return true;
	
	// generators can never be identity
	if( num_inputs() == 0)
		return false;

	return do_is_identity();
}

bool node_t::do_is_identity() const { return false;}

// hash
void node_t::clear_hash()
{
	hash_generator().reset();

    set_cacheable( !is_frame_varying());

    if( cacheable())
    {
        BOOST_FOREACH( input_plug_t& i, input_plugs())
        {
            if( i.connected())
            {
                if( !i.input()->cacheable())
                {
                    set_cacheable( false);
                    return;
                }
            }
        }
    }
	
	frames_needed().clear();
}

void node_t::calc_hash_str( const render::context_t& context)
{
	RAMEN_ASSERT( hash_generator().empty());

    if( !cacheable())
        return;

	bool valid = is_valid();

	if( valid && is_identity())
	{
		if( num_inputs() > 0 && input())
			hash_generator() << input()->hash_str();
		
		return;
	}
	
    hash_generator() << typeid( *this).name();

    if( !valid)
    {
		add_context_to_hash_string( context);
        return;
    }

	// handle the case when all inputs are optional and disconnected.
	if( num_inputs() != 0)
	{
		bool all_optional = true;
		bool not_connected = true;

		for( int i = 0; i < num_inputs(); ++i)
		{
			if( !input_plugs()[i].optional())
				all_optional = false;
		
			if( input(i))
				not_connected = false;
		}
	
		if( all_optional && not_connected)
			add_context_to_hash_string( context);
	}

	// for each needed input frame...
	calc_frames_needed( context);
	
	if( !frames_needed_.empty() && !ignored())
		add_needed_frames_to_hash( context);
	else
	{
		for( int i = 0; i < num_inputs(); ++i)
			if( input( i) && include_input_in_hash( i))
				hash_generator() << i << input( i)->hash_str();
	}
	
	do_calc_hash_str( context);
}

void node_t::do_calc_hash_str( const render::context_t& context)
{
	param_set().add_to_hash( hash_generator());
}

std::string node_t::hash_str() const { return hash_generator().str();}

const hash_generator_t::digest_type& node_t::digest() { return hash_generator().digest();}

void node_t::add_context_to_hash_string( const render::context_t& context)
{
	hash_generator() << context.subsample;
	hash_generator() << context.motion_blur_extra_samples;
	hash_generator() << context.motion_blur_shutter_factor;	
}

bool node_t::include_input_in_hash( int num) const { return true;}

void node_t::add_needed_frames_to_hash( const render::context_t& context)
{
	const_frames_needed_iterator it( frames_needed().begin()), end( frames_needed().end());
	
	while( it != end)
	{
		float cur_frame = it->first;
		render::context_t new_context( context);
		new_context.frame = cur_frame;

		while( 1)
		{
			node_t *n = input( it->second);
			depth_first_inputs_search( *n, boost::bind( &node_t::set_frame, _1, cur_frame));
			depth_first_inputs_search( *n, boost::bind( &node_t::clear_hash, _1));
			depth_first_inputs_search( *n, boost::bind( &node_t::calc_hash_str, _1, new_context));
			hash_generator() << it->second << n->hash_str();
			++it;
			
			if( it == end || it->first != cur_frame)
				break;
		}
	}

	// restore original time here
	for( int i = 0; i < num_inputs(); ++i)
	{
		if( node_t *n = input( i))
		{
			depth_first_inputs_search( *n, boost::bind( &node_t::set_frame, _1, context.frame));
			depth_first_inputs_search( *n, boost::bind( &node_t::clear_hash, _1));
			depth_first_inputs_search( *n, boost::bind( &node_t::calc_hash_str, _1, context));
		}
	}
}

// cache
bool node_t::is_frame_varying() const { return false;}

// ui
const char *node_t::help_string() const
{
    RAMEN_ASSERT( metaclass());
    return metaclass()->help;
}

void node_t::convert_relative_paths( const boost::filesystem::path& old_base, const boost::filesystem::path& new_base)
{
    adobe::for_each( param_set(), boost::bind( &param_t::convert_relative_paths, _1, old_base, new_base));
}

void node_t::make_paths_absolute()
{
    adobe::for_each( param_set(), boost::bind( &param_t::make_paths_absolute, _1));
}

void node_t::make_paths_relative()
{
    adobe::for_each( param_set(), boost::bind( &param_t::make_paths_relative, _1));
}

// serialization
void node_t::read( const serialization::yaml_node_t& node, const std::pair<int,int>& version)
{
	std::string n;
	node.get_value( "name", n);
	
	RAMEN_ASSERT( is_string_valid_identifier( n));
	
	if( !is_string_valid_identifier( n))
		throw std::runtime_error( "Bad name in node_t");

	set_name( n);
	
	if( !node.get_optional_value( "comp_pos", loc_))
		set_autolayout( true);

	// create needed extra inputs if needed.
	if( variable_num_inputs())
	{
		int num_ins = num_inputs();
		node.get_optional_value( "num_inputs", num_ins);

		while( num_ins != num_inputs())
			add_new_input_plug();
	}

	bool flag = false;
	if( node.get_optional_value( "ignored", flag))
		set_ignored( flag);

	serialization::yaml_node_t prms( node.get_node( "params"));
	param_set().read( prms);

	do_read( node, version);
}

void node_t::do_read( const serialization::yaml_node_t& node, const std::pair<int,int>& version) {}

void node_t::write( serialization::yaml_oarchive_t& out) const
{
    RAMEN_ASSERT( metaclass() && "Trying to serialize an abstract node");
	out.begin_map();
		write_node_info( out);
        param_set().write( out);
        do_write( out);
	out.end_map();
}

void node_t::do_write( serialization::yaml_oarchive_t& out) const {}

void node_t::write_node_info( serialization::yaml_oarchive_t& out) const
{
	out << YAML::Key << "class" << YAML::Value;
	out.flow();
		out.begin_seq();
		out << metaclass()->id
			<< metaclass()->major_version << metaclass()->minor_version;
		out.end_seq();

	out << YAML::Key << "name"  << YAML::DoubleQuoted << YAML::Value << name();
	out << YAML::Key << "comp_pos" << YAML::Value << location();
	
	if( ignored())
		out << YAML::Key << "ignored" << YAML::Value << true;

	if( variable_num_inputs())
		out << YAML::Key << "num_inputs" << YAML::Value << num_inputs();
}

} // namespace
