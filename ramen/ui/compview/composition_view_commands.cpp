// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/python/python.hpp>

#include<ramen/ui/compview/composition_view_commands.hpp>

#include<boost/bind.hpp>

#include<ramen/app/application.hpp>
#include<ramen/app/document.hpp>

#include<ramen/nodes/graph_algorithm.hpp>

namespace ramen
{
namespace undo
{

connect_command_t::connect_command_t( node_t *src, node_t *dest, int port) : command_t( "Connect"), src_(src), dest_(dest), port_(port)
{
    prev_src_ = dest_->input( port_);
}

void connect_command_t::undo()
{
    /*
    app().document().composition().disconnect( src_, dest_, port_);

    if( prev_src_)
        app().document().composition().connect( prev_src_, dest_, port_);

    dest_->notify();
    */
    command_t::undo();
}

void connect_command_t::redo()
{
    /*
    app().document().composition().connect( src_, dest_, port_);
    dest_->notify();
    */
    command_t::redo();
}

disconnect_command_t::disconnect_command_t( node_t *src, node_t *dest, int port) : command_t( "Disconnect"), src_(src), dest_(dest), port_(port) {}

void disconnect_command_t::undo()
{
    //app().document().composition().connect( src_, dest_, port_);
    //dest_->notify();
    command_t::undo();
}

void disconnect_command_t::redo()
{
    //app().document().composition().disconnect( src_, dest_, port_);
    //dest_->notify();
    command_t::redo();
}

} // namespace
} // namespace
