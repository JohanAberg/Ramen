// Copyright (c) 2010 Esteban Tovagliari

#ifndef RAMEN_FILE_PARAM_HPP
#define	RAMEN_FILE_PARAM_HPP

#include<ramen/params/static_param.hpp>

#ifndef RAMEN_NO_GUI
	#include<QPointer>
	#include<ramen/ui/widgets/line_edit_fwd.hpp>

	class QPushButton;
#endif
	
namespace ramen
{

class RAMEN_API file_param_t : public static_param_t
{
    Q_OBJECT
    
public:

    explicit file_param_t( const std::string& name);

    void set_default_value( const boost::filesystem::path& p);

    void set_value( const boost::filesystem::path& p, change_reason reason = user_edited);

    void set_is_input( bool b) { is_input_ = b;}

    bool empty() const;
	bool file_exists() const;
	
	// extensions
	void set_extension_list_string( const std::string& str) { ext_list_string_ = str;}
	
    std::string extension() const;
    void set_extension( const std::string& ext);

    // paths
    virtual void convert_relative_paths( const boost::filesystem::path& old_base, const boost::filesystem::path& new_base);
	virtual void make_paths_absolute();
	virtual void make_paths_relative();

protected:

    file_param_t( const file_param_t& other);
    void operator=( const file_param_t& other);

private:

    virtual param_t *do_clone() const { return new file_param_t( *this);}

    virtual void do_add_to_hash( hash_generator_t& hash_gen) const;

	virtual boost::python::object to_python( const poly_param_value_t& v) const;
	virtual poly_param_value_t from_python( const boost::python::object& obj) const;

	virtual void do_read( const serialization::yaml_node_t& node);
    virtual void do_write( serialization::yaml_oarchive_t& out) const;
	
    virtual void do_update_widgets();
    virtual void do_enable_widgets( bool e);

    void update_input_text( const boost::filesystem::path& p);
    void update_input_text();

#ifndef RAMEN_NO_GUI
	    virtual QWidget *do_create_widgets() RAMEN_WARN_UNUSED_RESULT;
	
		bool is_input_;
	
		QPointer<ui::line_edit_t> input_;
		QPointer<QPushButton> button_;
	
		std::string ext_list_string_;
		
	private Q_SLOTS:
	
		void select_pushed();
		void text_changed();
#endif
};

} // namespace

#endif
