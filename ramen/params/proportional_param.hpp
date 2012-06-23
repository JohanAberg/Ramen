// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_PROPORTIONAL_PARAM_HPP
#define	RAMEN_PROPORTIONAL_PARAM_HPP

#include<ramen/params/numeric_param.hpp>

#include<OpenEXR/ImathVec.h>

namespace ramen
{
namespace params
{

class RAMEN_API proportional_param_t : public numeric_param_t
{
public:

    explicit proportional_param_t( const std::string& name);

    bool proportional() const	    { return flags() & proportional_bit;}
    void set_proportional( bool p);

protected:

    proportional_param_t( const proportional_param_t& other);
    void operator=( const proportional_param_t& other);

    static Imath::V3f proportional_factor;
};

} // namespace
} // namespace

#endif
