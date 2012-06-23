// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_IMAGE_FORMAT_PARAM_HPP
#define	RAMEN_IMAGE_FORMAT_PARAM_HPP

#include<ramen/params/static_param.hpp>

#include<ramen/image/format.hpp>

namespace ramen
{
namespace params
{

class RAMEN_API image_format_param_t : public static_param_t
{
public:

    explicit image_format_param_t( const std::string& name);

    void set_value( const image::format_t& format, change_reason = user_edited);

protected:

    image_format_param_t( const image_format_param_t& other);
    void operator=( const image_format_param_t& other);

private:

    virtual void do_init();
    virtual param_t *do_clone() const;

    virtual void do_add_to_hash( hash::generator_t& hash_gen) const;

    virtual void do_read( const serialization::yaml_node_t& node);
    virtual void do_write( serialization::yaml_oarchive_t& out) const;
};

} // namespace
} // namespace

#endif
