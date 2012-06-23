// Copyright (c) 2010 Esteban Tovagliari

#include<ramen/nodes/node.hpp>

#include<vector>
#include<stdexcept>
#include<algorithm>

#include<boost/bind.hpp>
#include<boost/foreach.hpp>
#include<boost/range/algorithm/for_each.hpp>

#include<ramen/nodes/world_node.hpp>

#include<ramen/serialization/yaml_iarchive.hpp>
#include<ramen/serialization/yaml_oarchive.hpp>

#include<ramen/util/string.hpp>
#include<ramen/util/flags.hpp>

namespace ramen
{
namespace nodes
{

node_t::node_t() : composite_parameterised_t(), flags_( 0) {}

node_t::node_t( const node_t& other) : composite_parameterised_t( other), outputs_( other.outputs_)
{
    boost::range::for_each( outputs_, boost::bind( &output_plug_t::set_parent_node, _1, this));
    flags_ = other.flags_;
    loc_ = other.loc_;
}

node_t::~node_t() {}

void node_t::init()
{
    create_plugs();
    create_params();
    create_manipulators();
    do_init();
}

void node_t::do_init() {}

void node_t::cloned()
{
    //for( int i = 0; i < num_inputs(); ++i)
    //    connected( 0, i);

    create_manipulators();
}

const composite_node_t *node_t::parent_node() const
{
    return dynamic_cast<const composite_node_t*>( parent());
}

composite_node_t *node_t::parent_node()
{
    return dynamic_cast<composite_node_t*>( parent());
}

// plugs
void node_t::create_plugs()
{
    RAMEN_ASSERT( world());

    do_create_plugs();
}

void node_t::do_create_plugs() {}

// visitor
void node_t::accept( visitor_t& v) { v.visit( this);}

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

bool node_t::ignored() const        { return util::test_flag( flags_, ignored_bit);}
void node_t::set_ignored( bool b)   { util::set_flag( flags_, ignored_bit, b );}

bool node_t::plugin_error() const       { return util::test_flag( flags_, plugin_error_bit);}
void node_t::set_plugin_error( bool b)  { util::set_flag( flags_, plugin_error_bit, b );}

bool node_t::autolayout() const         { return util::test_flag( flags_, autolayout_bit);}
void node_t::set_autolayout( bool b)    { util::set_flag( flags_, autolayout_bit, b );}

bool node_t::cacheable() const          { return flags_ & cacheable_bit;}
void node_t::set_cacheable( bool b)     { util::set_flag( flags_, cacheable_bit, b );}

bool node_t::notify_dirty() const       { return util::test_flag( flags_, notify_dirty_bit);}
void node_t::set_notify_dirty( bool b)  { util::set_flag( flags_, notify_dirty_bit, b );}

bool node_t::ui_invisible() const       { return flags_ & ui_invisible_bit;}
void node_t::set_ui_invisible( bool b)  { util::set_flag( flags_, ui_invisible_bit, b );}

// inputs

void node_t::add_input_plug( const name_t& id, bool optional, const Imath::Color3c &color, const std::string &tooltip)
{
    if( find_input( id) == -1)
        throw std::runtime_error( util::concat_strings( "Duplicated input plud id: ", id.c_str()));

    inputs_.push_back( new input_plug_t( id, optional, color, tooltip ));
}

int node_t::find_input( const name_t& id) const
{
    int index = 0;
    BOOST_FOREACH( const input_plug_t& i, input_plugs())
    {
        if( i.id().c_str() == id.c_str() )
            return index;

        ++index;
    }

    return -1;
}

const input_plug_t& node_t::input_plug( const name_t& id) const
{
    int index = find_input( id);

    if( index < 0)
        throw std::runtime_error( util::concat_strings( "Couldn't find input plug id: ", id.c_str()));

    return inputs_[index];
}

input_plug_t& node_t::input_plug( const name_t& id)
{
    int index = find_input( id);

    if( index < 0)
        throw std::runtime_error( util::concat_strings( "Couldn't find input plug id: ", id.c_str()));

    return inputs_[index];
}

const node_t *node_t::input( const name_t& id) const
{
    int index = find_input( id);

    if( index < 0)
        throw std::runtime_error( util::concat_strings( "Couldn't find input plug id: ", id.c_str()));

    return inputs_[index].input_node();
}

node_t *node_t::input( const name_t& id)
{
    int index = find_input( id);

    if( index < 0)
        throw std::runtime_error( util::concat_strings( "Couldn't find input plug id: ", id.c_str()));

    return inputs_[index].input_node();
}

// outputs
int node_t::find_output( const name_t& id) const
{
    int index = 0;
    BOOST_FOREACH( const output_plug_t& i, output_plugs())
    {
        if( i.id().c_str() == id.c_str() )
            return index;

        ++index;
    }

    return -1;
}

const output_plug_t& node_t::output_plug( const name_t& id) const
{
    int index = find_output( id);

    if( index < 0)
        throw std::runtime_error( util::concat_strings( "Couldn't find output plug id: ", id.c_str()));

    return outputs_[index];
}

output_plug_t& node_t::output_plug( const name_t& id)
{
    int index = find_output( id);

    if( index < 0)
        throw std::runtime_error( util::concat_strings( "Couldn't find output plug id: ", id.c_str()));

    return outputs_[index];
}

void node_t::add_output_plug(const name_t& id, const Imath::Color3c& color, const std::string& tooltip )
{
    if( find_output( id) == -1)
        throw std::runtime_error( util::concat_strings( "Duplicated output plug id: ", id.c_str()));

    outputs_.push_back( new output_plug_t( this, id, color, tooltip ));
}

void node_t::add_to_dependency_graph()
{
    world_node_t *w = world();
    RAMEN_ASSERT( w);

    BOOST_FOREACH( input_plug_t& in, input_plugs())
        w->dependency_graph().add_node( dynamic_cast<dependency::node_t*>( &in));

    BOOST_FOREACH( output_plug_t& out, output_plugs())
        w->dependency_graph().add_node( dynamic_cast<dependency::node_t*>( &out));

    param_set().add_params_to_dependency_graph( w->dependency_graph());
    do_add_to_dependency_graph();
}

void node_t::do_add_to_dependency_graph() {}

bool node_t::accept_connection( node_t *src, const name_t& src_port, const name_t& dst_port) const
{
    return do_accept_connection( src, src_port, dst_port);
}

bool node_t::do_accept_connection( node_t *src, const name_t& src_port, const name_t& dst_port) const { return true;}

void node_t::connected(node_t *src, const name_t& src_port, const name_t& dst_port)
{
    /*
    if( variable_num_inputs() && src != 0 && port == num_inputs()-1)
        add_new_input_plug();
    */

    do_connected( src, src_port, dst_port);
}

void node_t::do_connected( node_t *src, const name_t& src_port, const name_t& dst_port) {}

void node_t::add_new_input_plug()
{
    RAMEN_ASSERT( 0 );
    /*
    add_input_plug( input_plug_info_t( ui::palette_t::instance().color("back plug")), true);
    reconnect_node();
    */
}

// params
void node_t::param_edit_finished() { notify();}

void node_t::notify()
{
    // use the dependency graph here.
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

// ui
const char *node_t::help_string() const
{
    RAMEN_ASSERT( class_metadata());
    return class_metadata()->help;
}

void node_t::convert_relative_paths( const boost::filesystem::path& old_base, const boost::filesystem::path& new_base)
{
    boost::range::for_each( param_set(), boost::bind( &param_t::convert_relative_paths, _1, old_base, new_base));
}

void node_t::make_paths_absolute()
{
    boost::range::for_each( param_set(), boost::bind( &param_t::make_paths_absolute, _1));
}

void node_t::make_paths_relative()
{
    boost::range::for_each( param_set(), boost::bind( &param_t::make_paths_relative, _1));
}

// serialization
void node_t::read(const serialization::yaml_node_t& in, const std::pair<int,int>& version)
{
    std::string n;
    in.get_value( "name", n);

    RAMEN_ASSERT( util::is_string_valid_identifier( n));

    if( !util::is_string_valid_identifier( n))
        throw std::runtime_error( "Bad name in node_t");

    set_name( n);

    if( !in.get_optional_value( "comp_pos", loc_))
        set_autolayout( true);

    // create needed extra inputs if needed.
    if( variable_num_inputs())
    {
        int num_ins = num_inputs();
        in.get_optional_value( "num_inputs", num_ins);

        while( num_ins != num_inputs())
            add_new_input_plug();
    }

    bool flag = false;
    if( in.get_optional_value( "ignored", flag))
        set_ignored( flag);

    serialization::yaml_node_t prms( in.get_node( "params"));
    param_set().read( prms);

    do_read( in, version);
}

void node_t::do_read( const serialization::yaml_node_t& in, const std::pair<int,int>& version) {}

void node_t::write( serialization::yaml_oarchive_t& out) const
{
    RAMEN_ASSERT( class_metadata() && "Trying to serialize an abstract node");
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
        out << class_metadata()->id
            << class_metadata()->major_version << class_metadata()->minor_version;
        out.end_seq();

    out << YAML::Key << "name"  << YAML::DoubleQuoted << YAML::Value << name();
    out << YAML::Key << "comp_pos" << YAML::Value << location();

    if( ignored())
        out << YAML::Key << "ignored" << YAML::Value << true;

    if( variable_num_inputs())
        out << YAML::Key << "num_inputs" << YAML::Value << num_inputs();
}

node_t *new_clone( const node_t& other)
{
    return dynamic_cast<node_t*>( new_clone( dynamic_cast<const parameterised_t&>( other)));
}

} // namespace
} // namespace
