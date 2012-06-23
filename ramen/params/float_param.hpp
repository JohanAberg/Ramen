// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_FLOAT_PARAM_HPP
#define	RAMEN_FLOAT_PARAM_HPP

#include<ramen/params/numeric_param.hpp>

namespace ramen
{
namespace params
{

class RAMEN_API float_param_t : public numeric_param_t
{
public:

    explicit float_param_t( const std::string& name);

    void set_default_value( float x);

    virtual poly_regular_t value_at_frame( float frame) const;

    void set_value( float x, change_reason reason = user_edited);
    void set_value_at_frame( float x, float frame, change_reason reason = user_edited);

    void set_absolute_value( float x, change_reason reason = user_edited);
    void set_absolute_value_at_frame( float x, float frame, change_reason reason = user_edited);

    float derive( float time) const;
    float integrate( float time1, float time2) const;

protected:

    float_param_t( const float_param_t& other);
    void operator=( const float_param_t& other);

private:

    void private_init();

    virtual param_t *do_clone() const { return new float_param_t( *this);}

    virtual void do_add_to_hash( hash::generator_t& hash_gen) const;

    virtual boost::python::object to_python( const poly_regular_t& v) const;
    virtual poly_regular_t from_python( const boost::python::object& obj) const;
};

} // namespace
} // namespace

#endif
