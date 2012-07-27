// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_COMPOSITE_PARAM_HPP
#define	RAMEN_COMPOSITE_PARAM_HPP

#include<ramen/config.hpp>

#include<ramen/params/param.hpp>

#include<memory>

#include<boost/ptr_container/ptr_vector.hpp>

#include<ramen/undo/command.hpp>

namespace ramen
{
namespace params
{

class RAMEN_API composite_param_t : public param_t
{
public:

    composite_param_t();
    explicit composite_param_t( const std::string& name);

    void set_create_track( bool b) { create_track_ = b;}

    const boost::ptr_vector<param_t>& params() const	{ return params_;}
    boost::ptr_vector<param_t>& params()				{ return params_;}

    template<class T>
    void add_param( std::auto_ptr<T> p) { do_add_param( p.release());}

    const param_t *find(const base::name_t& id) const;
    param_t *find( const base::name_t& id);

protected:

    composite_param_t( const composite_param_t& other);
    void operator=( const composite_param_t& other);

private:

    virtual void do_init();
    virtual param_t *do_clone() const { return new composite_param_t( *this);}

    virtual void do_set_param_set( param_set_t *parent);

    void do_add_param( param_t *p);

    virtual void do_set_frame( float frame);
    virtual void do_evaluate( float frame);

    virtual void do_add_to_hash( hash::generator_t& hash_gen) const;

    virtual void do_create_tracks( anim::track_t *parent);

    // paths
    virtual void do_convert_relative_paths( const boost::filesystem::path& old_base,
                                            const boost::filesystem::path& new_base);

    virtual void do_make_paths_absolute();
    virtual void do_make_paths_relative();

    // util
    virtual void do_apply_function( const boost::function<void ( param_t*)> *f);

    boost::ptr_vector<param_t> params_;
    bool create_track_;
};

} // namespace
} // namespace

#endif
