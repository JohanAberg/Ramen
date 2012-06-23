// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_BUTTON_PARAM_HPP
#define	RAMEN_BUTTON_PARAM_HPP

#include<ramen/params/param.hpp>

namespace ramen
{
namespace params
{

class RAMEN_API button_param_t : public param_t
{
public:

    explicit button_param_t( const std::string& name);

protected:

    button_param_t( const button_param_t& other);
    void operator=( const button_param_t& other);

private:

    virtual param_t *do_clone() const { return new button_param_t( *this);}
};

} // namespace
} // namespace

#endif

