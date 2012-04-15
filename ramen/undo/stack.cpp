// Copyright (c) 2010 Esteban Tovagliari

#include<ramen/undo/stack.hpp>

namespace ramen
{
namespace undo
{

stack_impl::stack_impl() {}
stack_impl::~stack_impl() {}

void stack_impl::clear()
{
    undo_stack_.clear();
    redo_stack_.clear();
}

void stack_impl::erase_last_command() { undo_stack_.pop_back();}

void stack_impl::undo()
{
    RAMEN_ASSERT( !undo_stack_.empty());
	
    undo_stack_.back().undo();
    boost::ptr_deque<command_t>::auto_type c( undo_stack_.pop_back());
    redo_stack_.push_back( c.release());
}

void stack_impl::redo()
{
    RAMEN_ASSERT( !redo_stack_.empty());

    redo_stack_.back().redo();
    boost::ptr_deque<command_t>::auto_type c( redo_stack_.pop_back());
    undo_stack_.push_back( c.release());
}

} // namespace
} // namespace
