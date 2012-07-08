// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_TRANSFORM2_PARAM_HPP
#define	RAMEN_TRANSFORM2_PARAM_HPP

#include<ramen/params/composite_param.hpp>

#include<OpenEXR/ImathMatrix.h>

#include<ramen/nodes/node_fwd.hpp>
#include<ramen/nodes/image/track/tracker_node_fwd.hpp>

namespace ramen
{
namespace params
{

class RAMEN_API transform2_param_t : public composite_param_t
{
public:

    typedef image::matrix3_t matrix_type;

    transform2_param_t( const std::string& name, const std::string& id);

    const param_t& center_param() const;
    param_t& center_param();
    void move_center( const Imath::V2f& c, change_reason reason = user_edited);

    const param_t& translate_param() const;
    param_t& translate_param();
    void translate( const Imath::V2f& t, change_reason reason = user_edited);

    const param_t& scale_param() const;
    param_t& scale_param();

    const param_t& rotate_param() const;
    param_t& rotate_param();

    matrix_type matrix_at_frame( float frame, float aspect = 1.0f, int subsample = 1) const;
    matrix_type xform_blur_matrix_at_frame( float frame, float t, float aspect = 1.0f, int subsample = 1) const;

    void apply_track( float start_frame, float end_frame, const image::tracker_node_t *tracker,
                      image::apply_track_mode mode, image::apply_track_use use);

protected:

    transform2_param_t( const transform2_param_t& other);
    void operator=( const transform2_param_t& other);

private:

    virtual param_t *do_clone() const { return new transform2_param_t( *this);}
};

} // namespace
} // namespace

#endif
