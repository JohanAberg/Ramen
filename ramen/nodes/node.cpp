// Copyright (c) 2010 Esteban Tovagliari

#include<ramen/nodes/node.hpp>

#include<vector>
#include<stdexcept>
#include<algorithm>

#include<boost/bind.hpp>
#include<boost/foreach.hpp>
#include<boost/range/algorithm/for_each.hpp>

#include<ramen/nodes/world_node.hpp>

#include<ramen/util/string.hpp>
#include<ramen/util/flags.hpp>

namespace ramen
{
namespace nodes
{
namespace
{

struct add_depends_all_out
{
    add_depends_all_out( nodes::node_t *n) : n_( n)
    {
        BOOST_ASSERT( n);
    }

    void operator()( dependency::node_t& src)
    {
        BOOST_FOREACH( output_plug_t& oplug, n_->output_plugs())
        {
            n_->add_dependency( &src, &oplug);
        }
    }

    void operator()( dependency::node_t *src) { (*this)( *src);}

    nodes::node_t *n_;
};

} // unnamed

node_t::node_t() : params::parameterised_t(), flags_( 0) {}

node_t::node_t( const node_t& other) : params::parameterised_t( other),
                                        inputs_( other.inputs_),
                                        outputs_( other.outputs_)
{
    boost::range::for_each( outputs_, boost::bind( &output_plug_t::set_parent_node, _1, this));
    flags_ = other.flags_;
    loc_ = other.loc_;

    BOOST_FOREACH( input_plug_t& plug, input_plugs())
        add_dependency_node( &plug);

    BOOST_FOREACH( output_plug_t& plug, output_plugs())
        add_dependency_node( &plug);
}

node_t::~node_t() {}

void node_t::init()
{
    create_plugs();
    create_params();
    add_dependencies();
    create_manipulators();
    do_init();
}

void node_t::do_init() {}

void node_t::cloned()
{
    add_dependencies();
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

bool node_t::ui_invisible() const       { return flags_ & ui_invisible_bit;}
void node_t::set_ui_invisible( bool b)  { util::set_flag( flags_, ui_invisible_bit, b );}

// inputs

void node_t::add_input_plug( const base::name_t& id, bool optional, const Imath::Color3c &color, const std::string &tooltip)
{
    if( find_input( id) != -1)
        throw std::runtime_error( util::concat_strings( "Duplicated input plud id: ", id.c_str()));

    std::auto_ptr<input_plug_t> plug( new input_plug_t( id, optional, color, tooltip ));
    add_dependency_node( plug.get());
    inputs_.push_back( plug);
}

int node_t::find_input( const base::name_t& id) const
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

const input_plug_t& node_t::input_plug( const base::name_t& id) const
{
    int index = find_input( id);

    if( index < 0)
        throw std::runtime_error( util::concat_strings( "Couldn't find input plug id: ", id.c_str()));

    return inputs_[index];
}

input_plug_t& node_t::input_plug( const base::name_t& id)
{
    int index = find_input( id);

    if( index < 0)
        throw std::runtime_error( util::concat_strings( "Couldn't find input plug id: ", id.c_str()));

    return inputs_[index];
}

const node_t *node_t::input( const base::name_t& id) const
{
    int index = find_input( id);

    if( index < 0)
        throw std::runtime_error( util::concat_strings( "Couldn't find input plug id: ", id.c_str()));

    return inputs_[index].input_node();
}

node_t *node_t::input( const base::name_t& id)
{
    int index = find_input( id);

    if( index < 0)
        throw std::runtime_error( util::concat_strings( "Couldn't find input plug id: ", id.c_str()));

    return inputs_[index].input_node();
}

// outputs
int node_t::find_output( const base::name_t& id) const
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

const output_plug_t& node_t::output_plug( const base::name_t& id) const
{
    int index = find_output( id);

    if( index < 0)
        throw std::runtime_error( util::concat_strings( "Couldn't find output plug id: ", id.c_str()));

    return outputs_[index];
}

output_plug_t& node_t::output_plug( const base::name_t& id)
{
    int index = find_output( id);

    if( index < 0)
        throw std::runtime_error( util::concat_strings( "Couldn't find output plug id: ", id.c_str()));

    return outputs_[index];
}

void node_t::add_output_plug(const base::name_t& id, const Imath::Color3c& color, const std::string& tooltip )
{
    if( find_output( id) != -1)
        throw std::runtime_error( util::concat_strings( "Duplicated output plug id: ", id.c_str()));

    std::auto_ptr<output_plug_t> plug( new output_plug_t( this, id, color, tooltip ));
    add_dependency_node( plug.get());
    outputs_.push_back( plug);
}

bool node_t::accept_connection( node_t *src, const base::name_t& src_port, const base::name_t& dst_port) const
{
    return do_accept_connection( src, src_port, dst_port);
}

bool node_t::do_accept_connection( node_t *src, const base::name_t& src_port, const base::name_t& dst_port) const { return true;}

void node_t::connected(node_t *src, const base::name_t& src_port, const base::name_t& dst_port)
{
    /*
    if( variable_num_inputs() && src != 0 && port == num_inputs()-1)
        add_new_input_plug();
    */

    do_connected( src, src_port, dst_port);
}

void node_t::do_connected( node_t *src, const base::name_t& src_port, const base::name_t& dst_port) {}

void node_t::add_new_input_plug()
{
    RAMEN_ASSERT( 0 );
    /*
    add_input_plug( input_plug_info_t( ui::palette_t::instance().color("back plug")), true);
    reconnect_node();
    */
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

void node_t::add_dependencies()
{
    do_add_dependencies();
}

void node_t::do_add_dependencies()
{
    // by default, add all posible dependencies
    BOOST_FOREACH( input_plug_t& iplug, input_plugs())
    {
        BOOST_FOREACH( output_plug_t& oplug, output_plugs())
            add_dependency( &iplug, &oplug);
    }

    add_depends_all_out fun( this);
    for_each_param( fun);
}

void node_t::do_propagate_dirty_flags()
{
    BOOST_FOREACH( output_plug_t& plug, output_plugs())
    {
        if( plug.dirty())
        {
            BOOST_FOREACH( output_plug_t::connection_type& c, plug.connections())
            {
                node_t *dst = boost::get<0>( c);
                dst->input_plug( boost::get<1>( c)).set_dirty();
                dst->propagate_dirty_flags();
            }
        }
    }
}

node_t *new_clone( const node_t& other)
{
    node_t *n = dynamic_cast<node_t*>( new_clone( dynamic_cast<const params::parameterised_t&>( other)));
    n->cloned();
    return n;
}

} // namespace
} // namespace
