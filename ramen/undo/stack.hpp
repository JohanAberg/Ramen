// Copyright (c) 2010 Esteban Tovagliari

#ifndef RAMEN_UNDO_STACK_HPP
#define RAMEN_UNDO_STACK_HPP

#include<vector>
#include<stack>

#include<boost/noncopyable.hpp>
#include<boost/ptr_container/ptr_deque.hpp>

#include<loki/Singleton.h>

#include<ramen/assert.hpp>

#include<ramen/undo/command.hpp>

namespace ramen
{
namespace undo
{

class stack_impl : boost::noncopyable
{
public:

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

    stack_impl();
    ~stack_impl();

    friend struct Loki::CreateUsingNew<stack_impl>;

    boost::ptr_deque<command_t> undo_stack_;
    boost::ptr_deque<command_t> redo_stack_;
};

typedef Loki::SingletonHolder<stack_impl> stack_t;

} // namespace
} // namespace

#endif
