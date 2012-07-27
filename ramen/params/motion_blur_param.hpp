// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_MOTION_BLUR_PARAM_HPP
#define RAMEN_MOTION_BLUR_PARAM_HPP

#include<ramen/params/static_param.hpp>

#include<ramen/params/motion_blur_info.hpp>

namespace ramen
{
namespace params
{

class RAMEN_API motion_blur_param_t : public static_param_t
{
public:

    explicit motion_blur_param_t( const std::string& name);

    bool motion_blur_enabled() const;

    motion_blur_info_t::loop_data_t loop_data( float time, int extra_samples, float shutter_factor) const;

protected:

    motion_blur_param_t( const motion_blur_param_t& other);
    void operator=( const motion_blur_param_t& other);

private:

    virtual param_t *do_clone() const { return new motion_blur_param_t( *this);}

    virtual void do_add_to_hash( hash::generator_t& hash_gen) const;
};

} // namespace
} // namespace

#endif
