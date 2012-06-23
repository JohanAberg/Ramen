// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_COLOR_PARAM_HPP
#define	RAMEN_COLOR_PARAM_HPP

#include<ramen/params/animated_param.hpp>

#include<OpenEXR/ImathColor.h>

namespace ramen
{
namespace params
{

class RAMEN_API color_param_t : public animated_param_t
{
public:

    explicit color_param_t( const std::string& name);

    void set_default_value( const Imath::Color4f& x);

    virtual poly_regular_t value_at_frame( float frame) const;

    void set_value( const Imath::Color4f& x, change_reason reason = user_edited);
    void set_value_at_frame( const Imath::Color4f& x, float frame, change_reason reason = user_edited);

    bool is_rgba() const	{ return is_rgba_;}
    void set_is_rgba( bool b)	{ is_rgba_ = b;}

protected:

    color_param_t( const color_param_t& other);
    void operator=( const color_param_t& other);

private:

    void private_init();

    virtual param_t *do_clone() const { return new color_param_t( *this);}

    virtual void do_create_tracks( anim::track_t *parent);

    virtual void do_add_to_hash( hash::generator_t& hash_gen) const;

    virtual boost::python::object to_python( const poly_regular_t& v) const;
    virtual poly_regular_t from_python( const boost::python::object& obj) const;

    bool is_rgba_;
};

} // namespace
} // namespace

#endif
