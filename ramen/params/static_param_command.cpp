// Copyright (c) 2010 Esteban Tovagliari

#include<ramen/python/python.hpp>

#include<ramen/params/static_param_command.hpp>

#include<ramen/nodes/node.hpp>

namespace ramen
{

static_param_command_t::static_param_command_t( param_set_t& pset, const std::string& id) : undo::command_t( "Param Changed"), pset_( pset)
{
    id_ = id;
    set_done( true);
    param_t& p = pset_.find( id_);
    old_value_ = p.value();
}

void static_param_command_t::undo()
{
    param_t& p = pset_.find( id_);
    new_value_ = p.value();
    p.value() = old_value_;
    p.update_widgets();
	p.emit_param_changed( param_t::user_edited);
    undo::command_t::undo();
}
	
void static_param_command_t::redo()
{
    param_t& p = pset_.find( id_);
    p.value() = new_value_;
    p.update_widgets();
	p.emit_param_changed( param_t::user_edited);
    undo::command_t::redo();
}

} // namespace
