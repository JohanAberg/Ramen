// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_COMPOSITE_PARAM_HPP
#define	RAMEN_COMPOSITE_PARAM_HPP

#include<ramen/config.hpp>

#include<ramen/params/param.hpp>

#include<memory>

#include<boost/ptr_container/ptr_vector.hpp>

namespace ramen
{
namespace params
{

class RAMEN_API composite_param_t : public param_t
{
public:

    composite_param_t();
    explicit composite_param_t( const std::string& name);

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

    // util
    virtual void do_apply_function( const boost::function<void ( param_t*)> *f);

    boost::ptr_vector<param_t> params_;
    bool create_track_;
};

} // namespace
} // namespace

#endif
