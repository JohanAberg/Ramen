// Copyright (c) 2012 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/python/python.hpp>

#include<ramen/nodes/graph_modifier.hpp>

#include<ramen/app/application.hpp>
#include<ramen/app/document.hpp>

#include<ramen/undo/stack.hpp>

namespace ramen
{
namespace nodes
{

struct graph_modifier_t::command_impl : public undo::command_t
{
};

graph_modifier_t::graph_modifier_t( bool undoable)
{
    if( undoable)
    {
    }
}

graph_modifier_t::~graph_modifier_t()
{
    if( command_.get())
    {
        command_->redo();
        app().document().undo_stack().push_back( command_);
    }
}

} // namespace
} // namespace
