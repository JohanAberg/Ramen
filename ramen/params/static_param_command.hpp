// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_STATIC_PARAM_COMMAND_HPP
#define RAMEN_STATIC_PARAM_COMMAND_HPP

#include<ramen/config.hpp>

#include<string>

#include<base/name.hpp>
#include<base/poly_regular.hpp>

#include<ramen/undo/command.hpp>

#include<ramen/params/param_set_fwd.hpp>
#include<ramen/nodes/node_fwd.hpp>

namespace ramen
{
namespace params
{

class RAMEN_API static_param_command_t : public undo::command_t
{
public:

    static_param_command_t( param_set_t& pset, const base::name_t& id);

    virtual void undo();
    virtual void redo();

protected:

    param_set_t& pset_;
    base::name_t id_;
    base::poly_regular_t old_value_, new_value_;
};

} // namespace
} // namespace

#endif
