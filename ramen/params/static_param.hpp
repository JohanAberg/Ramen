// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_STATIC_PARAM_HPP
#define	RAMEN_STATIC_PARAM_HPP

#include<ramen/params/param.hpp>

namespace ramen
{
namespace params
{

class RAMEN_API static_param_t : public param_t
{
public:

    explicit static_param_t( const std::string& name);

protected:

    static_param_t( const static_param_t& other);
    void operator=( const static_param_t& other);

    virtual std::auto_ptr<undo::command_t> do_create_command();
};

} // namespace
} // namespace

#endif
