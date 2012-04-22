// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UNDO_STACK_HPP
#define RAMEN_UNDO_STACK_HPP

#include<ramen/undo/stack_fwd.hpp>

#include<vector>
#include<stack>

#include<boost/noncopyable.hpp>
#include<boost/ptr_container/ptr_deque.hpp>

#include<ramen/assert.hpp>

#include<ramen/undo/command.hpp>

namespace ramen
{
namespace undo
{

class stack_t : boost::noncopyable
{
public:

    stack_t();
    ~stack_t();

    void clear();

    template<class T>
    void push_back( std::auto_ptr<T> c)
    {
		RAMEN_ASSERT( dynamic_cast<command_t*>( c.get()) != 0); // I think this is not needed...
		undo_stack_.push_back( c.release());

		if( undo_stack_.size() > 50)
			undo_stack_.pop_front();
    }

    void erase_last_command();

    command_t& last_undo_command() { return undo_stack_.back();}
    command_t& last_redo_command() { return redo_stack_.back();}

    void undo();
    void redo();

    bool undo_empty() const { return undo_stack_.empty();}
    bool redo_empty() const { return redo_stack_.empty();}
	
private:

    boost::ptr_deque<command_t> undo_stack_;
    boost::ptr_deque<command_t> redo_stack_;
};

} // namespace
} // namespace

#endif
