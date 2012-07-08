// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_NODE_HPP
#define	RAMEN_NODE_HPP

#include<ramen/python/python.hpp>

#include<ramen/nodes/node_fwd.hpp>

#include<vector>
#include<string>
#include<memory>
#include<set>
#include<utility>

#include<QWidget>

#include<ramen/params/parameterised.hpp>

#include<ramen/nodes/graph_color.hpp>
#include<ramen/nodes/plug.hpp>
#include<ramen/nodes/factory.hpp>
#include<ramen/nodes/visitor.hpp>

namespace ramen
{
namespace nodes
{

/*!
\ingroup nodes
\brief A node in Ramen's processing graph.
*/
class RAMEN_API node_t : public params::parameterised_t
{
public:

    virtual const class_metadata_t *class_metadata() const { return 0;}

    /// Constructor.
    node_t();

    /// Destructor.
    virtual ~node_t();

    /// Post constructor initialization.
    void init();

    /// Called for the new node, after being copied.
    virtual void cloned();

    /// Returns the composite node parent of this node.
    const composite_node_t *parent_node() const;

    /// Returns the composite node parent of this node.
    composite_node_t *parent_node();

    /// Creates this node plugs.
    void create_plugs();

    /// Returns the number of input plugs.
    std::size_t num_inputs() const { return inputs_.size();}

    const boost::ptr_vector<input_plug_t>& input_plugs() const { return inputs_;}
    boost::ptr_vector<input_plug_t>& input_plugs()             { return inputs_;}

    /// Returns the index of the input plug with the given id.
    int find_input( const base::name_t& id) const;

    /// Returns the input plug with the given id.
    const input_plug_t& input_plug( const base::name_t& id) const;

    /// Returns the input plug with the given id.
    input_plug_t& input_plug( const base::name_t& id);

    /// Returns a const pointer to the node connected to the input plug with the given id.
    const node_t *input( const base::name_t& id) const;

    /// Returns a pointer to the node connected to the input plug with the given id.
    node_t *input( const base::name_t& id);

    /// Returns a const pointer to the node connected to the input plug with the given id.
    template<class T>
    const T *input_as( const base::name_t& id) const
    {
        return dynamic_cast<const T*>( input( id));
    }

    /// Returns a pointer to the node connected to the input plug with the given id.
    template<class T>
    T *input_as( const base::name_t& id)
    {
        return dynamic_cast<T*>( input( id));
    }

    /// Returns the number of input plugs.
    std::size_t num_outputs() const { return outputs_.size();}

    const boost::ptr_vector<output_plug_t>& output_plugs() const { return outputs_;}
    boost::ptr_vector<output_plug_t>& output_plugs()             { return outputs_;}

    /// Returns the index of the output plug with the given id.
    int find_output( const base::name_t& id) const;

    /// Returns the output plug with the given id.
    const output_plug_t& output_plug( const base::name_t& id) const;

    /// Returns the output plug with the given id.
    output_plug_t& output_plug( const base::name_t& id);

    /// Adds this node components ( plugs & params) to the dg.
    virtual void add_to_dependency_graph();

    // graph
    graph_color_t graph_color() const            { return graph_color_;}
    void set_graph_color( graph_color_t c) const { graph_color_ = c;}

    /// visitor.
    virtual void accept( visitor_t& v);

    // ui
    const Imath::V2f& location() const		{ return loc_;}
    void set_location( const Imath::V2f& p)	{ loc_ = p;}
    void offset_location( const Imath::V2f& v)	{ loc_ += v;}

    // selection & flags
    bool selected() const;
    void select( bool state);
    void toggle_selection();

    bool plugin_error() const;
    void set_plugin_error( bool b);

    bool autolayout() const;
    void set_autolayout( bool b);

    bool cacheable() const;
    void set_cacheable( bool b);

    bool notify_dirty() const;
    void set_notify_dirty( bool b);

    bool ui_invisible() const;
    void set_ui_invisible( bool b);

    virtual void add_new_input_plug();

    // params
    virtual void param_edit_finished();

    // signals
    boost::signals2::signal<void ( node_t*)> changed;

    void notify();

    // connections
    virtual bool variable_num_inputs() const { return false;}

    bool accept_connection( node_t *src, const base::name_t& src_port, const base::name_t& dst_port) const;
    void connected( node_t *src, const base::name_t& src_port, const base::name_t& dst_port);

    // ignore
    bool ignored() const;
    void set_ignored( bool b);

    // valid
    bool is_valid() const;

    // user interface
    virtual const char *help_string() const;

    virtual std::auto_ptr<QWidget> create_toolbar() { return std::auto_ptr<QWidget>();}

    // paths
    virtual void convert_relative_paths( const boost::filesystem::path& old_base,
                                            const boost::filesystem::path& new_base);

    virtual void make_paths_absolute();
    virtual void make_paths_relative();

    // serialization
    void read( const serialization::yaml_node_t& in, const std::pair<int,int>& version);
    void write( serialization::yaml_oarchive_t& out) const;

protected:

    node_t( const node_t& other);
    void operator=( const node_t& other);

    void add_input_plug( const base::name_t& id, bool optional,
                         const Imath::Color3c& color, const std::string& tooltip );

    void add_output_plug( const base::name_t& id, const Imath::Color3c& color, const std::string& tooltip );

    bool is_valid_;

private:

    enum flag_bits
    {
        selected_bit			= 1 << 0,
        ignored_bit				= 1 << 1,
        plugin_error_bit		= 1 << 2,
        cacheable_bit			= 1 << 3,
        autolayout_bit			= 1 << 4,
        notify_dirty_bit		= 1 << 5,
        ui_invisible_bit		= 1 << 6,
        interacting_bit			= 1 << 7
    };

    /*!
        \brief Customization hook for node_t::init.
        For subclasses to implement.
    */
    virtual void do_init();

    /*!
        \brief Customization hook for node_t::create_plugs.
        For subclasses to implement.
    */
    virtual void do_create_plugs();

    /*!
        \brief Customization hook for node_t::add_to_dependency_graph.
        For subclasses to implement.
    */
    virtual void do_add_to_dependency_graph();

    /*!
        \brief Customization hook for node_t::accept_connection.
        For subclasses to implement.
    */
    virtual bool do_accept_connection( node_t *src, const base::name_t& src_port, const base::name_t& dst_port) const;

    /*!
        \brief Customization hook for node_t::connected.
        For subclasses to implement.
    */
    virtual void do_connected( node_t *src, const base::name_t& src_port, const base::name_t& dst_port);

    /*!
        \brief Customization hook for node_t::is_valid.
        For subclasses to implement.
    */
    virtual bool do_is_valid() const;

    /*!
        \brief Customization hook for node_t::read.
        Implement in subclasses to read extra data from node.
    */
    virtual void do_read(const serialization::yaml_node_t& in, const std::pair<int,int>& version);

    /*!
        \brief Customization hook for node_t::write.
        Implement in subclasses to write extra data to out.
    */
    virtual void do_write( serialization::yaml_oarchive_t& out) const;

    // serialization utils
    void write_node_info( serialization::yaml_oarchive_t& out) const;

    // data

    boost::ptr_vector<input_plug_t> inputs_;
    boost::ptr_vector<output_plug_t> outputs_;

    mutable graph_color_t graph_color_;

    boost::uint32_t flags_;
    Imath::V2f loc_;
};

/// Makes a copy of a node.
RAMEN_API node_t *new_clone( const node_t& other);

} // namespace
} // namespace

#endif
