// Copyright (c) 2010 Esteban Tovagliari

#ifndef RAMEN_COMPOSITE_PARAM_HPP
#define	RAMEN_COMPOSITE_PARAM_HPP

#include<ramen/config.hpp>

#include<ramen/params/param.hpp>

#include<memory>

#include<boost/ptr_container/ptr_vector.hpp>

#include<ramen/undo/command.hpp>

namespace ramen
{

class RAMEN_API composite_param_t : public param_t
{
    Q_OBJECT

public:

	composite_param_t();
    explicit composite_param_t( const std::string& name);

    virtual void init();

    virtual void set_param_set( param_set_t *parent);

	void set_create_track( bool b) { create_track_ = b;}
	
    const boost::ptr_vector<param_t>& params() const	{ return params_;}
    boost::ptr_vector<param_t>& params()				{ return params_;}

    template<class T>
    void add_param( std::auto_ptr<T> p) { do_add_param( p.release());}

	// time
	virtual void set_frame( float frame);
	
    // paths
    virtual void convert_relative_paths( const boost::filesystem::path& old_base,
                                            const boost::filesystem::path& new_base);

	virtual void make_paths_absolute();
	virtual void make_paths_relative();

	// util
	virtual void apply_function( const boost::function<void ( param_t*)>& f);
	
	// serialization
    virtual void write( serialization::yaml_oarchive_t& out) const;

protected:

    composite_param_t( const composite_param_t& other);
    void operator=( const composite_param_t& other);

private:

    virtual param_t *do_clone() const { return new composite_param_t( *this);}

    void do_add_param( param_t *p);

    virtual void do_evaluate( float frame);
	
	virtual void do_add_to_hash( hash_generator_t& hash_gen) const;
	
    virtual void do_update_widgets();
    virtual void do_enable_widgets( bool e);

    virtual void do_create_tracks( anim::track_t *parent);

    virtual void do_format_changed( const Imath::Box2i& new_format, float aspect, const Imath::V2f& proxy_scale);

    boost::ptr_vector<param_t> params_;
	bool create_track_;
	
	#ifndef RAMEN_NO_GUI
	protected:
	
	    void create_widgets_inside_widget( QWidget *parent);
		
	private:

		virtual QWidget *do_create_widgets() RAMEN_WARN_UNUSED_RESULT;
	#endif
};

} // namespace

#endif
