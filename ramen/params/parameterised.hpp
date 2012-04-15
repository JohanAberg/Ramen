// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_PARAMETERISED_HPP
#define	RAMEN_PARAMETERISED_HPP

#include<ramen/python/python.hpp>

#include<utility>

#include<boost/signals2/signal.hpp>

#include<OpenEXR/ImathBox.h>

#include<ramen/ref_counted.hpp>

#include<ramen/params/param_set.hpp>

#include<ramen/nodes/node_fwd.hpp>

#include<ramen/app/composition_fwd.hpp>

#include<ramen/anim/track_fwd.hpp>

namespace ramen
{

/**
\ingroup params
\brief Base class for objects that contains params.
*/
class RAMEN_API parameterised_t : public ref_counted_t
{
public:

    parameterised_t();
    parameterised_t( const parameterised_t& other);

    virtual ~parameterised_t();

    boost::signals2::signal<void ( parameterised_t*)> deleted;

    const std::string& name() const			{ return name_;}
    void set_name( const std::string& n);

    bool dont_persist_params() const        { return dont_persist_params_;}
    void set_dont_persist_params( bool b)   { dont_persist_params_ = b;}

    virtual bool autokey() const;
	virtual bool track_mouse() const;

	// composition
    const composition_t *composition() const;
    composition_t *composition();
	
	// node this parameterised belongs to
	virtual const node_t *node() const;
	virtual node_t *node();
	
    // params
    void create_params();

    const param_set_t& param_set() const    { return params_;}
    param_set_t& param_set()				{ return params_;}

    virtual const param_t& param( const std::string& identifier) const;
    virtual param_t& param( const std::string& identifier);

    template<class T>
    void add_param( std::auto_ptr<T> p) { param_set().add_param( p);}

    virtual void param_edit_finished() = 0;
	
	virtual void for_each_param( const boost::function<void ( param_t*)>& f);
	
	// anim & time
    void create_tracks( anim::track_t *root);
    void set_frame( float f);
	
	void update_widgets();
		
protected:
	
    void evaluate_params( float frame);
	
private:

    virtual void do_create_params();
	
    virtual void do_create_tracks( anim::track_t *parent);
    virtual void do_set_frame( float t) {}

	virtual void do_update_widgets();
	
    std::string name_;
    param_set_t params_;
    bool dont_persist_params_;	
};

} // namespace

#endif
