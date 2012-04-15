// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_MANIPULABLE_HPP
#define RAMEN_MANIPULABLE_HPP

#include<ramen/python/python.hpp>

#include<ramen/params/parameterised.hpp>

#include<memory>

#include<boost/ptr_container/ptr_vector.hpp>

#include<ramen/manipulators/manipulator.hpp>

namespace ramen
{

class RAMEN_API manipulable_t : public parameterised_t
{
public:

    manipulable_t();
    manipulable_t( const manipulable_t& other);

    virtual ~manipulable_t();

    void create_manipulators();

    template<class T>
    void add_manipulator( std::auto_ptr<T> p)
    {
        p->set_parent( this);
        manipulators_.push_back( p.release());
        active_ = manipulators_.end();
    }

    const boost::ptr_vector<manipulator_t>& manipulators() const    { return manipulators_;}
    boost::ptr_vector<manipulator_t>& manipulators()                { return manipulators_;}

    typedef boost::ptr_vector<manipulator_t>::const_iterator const_iterator;
    typedef boost::ptr_vector<manipulator_t>::iterator iterator;

    const_iterator begin() const    { return manipulators_.begin();}
    const_iterator end() const	    { return manipulators_.end();}

    iterator begin()	{ return manipulators_.begin();}
    iterator end()	{ return manipulators_.end();}

    void draw_overlay( const ui::paint_event_t& event) const;

    bool key_press_event( const ui::key_press_event_t& event);
    void key_release_event( const ui::key_release_event_t& event);

    void mouse_enter_event( const ui::mouse_enter_event_t& event);
    void mouse_leave_event( const ui::mouse_leave_event_t& event);

    bool mouse_press_event( const ui::mouse_press_event_t& event);
    void mouse_move_event( const ui::mouse_move_event_t& event);
    void mouse_drag_event( const ui::mouse_drag_event_t& event);
    void mouse_release_event( const ui::mouse_release_event_t& event);

protected:

    virtual void do_draw_overlay( const ui::paint_event_t& event) const;

    virtual bool do_key_press_event( const ui::key_press_event_t& event);
    virtual void do_key_release_event( const ui::key_release_event_t& event);

    virtual void do_mouse_enter_event( const ui::mouse_enter_event_t& event);
    virtual void do_mouse_leave_event( const ui::mouse_leave_event_t& event);

    virtual bool do_mouse_press_event( const ui::mouse_press_event_t& event);
    virtual void do_mouse_move_event( const ui::mouse_move_event_t& event);
    virtual void do_mouse_drag_event( const ui::mouse_drag_event_t& event);
    virtual void do_mouse_release_event( const ui::mouse_release_event_t& event);

private:

    virtual void do_create_manipulators();

    boost::ptr_vector<manipulator_t> manipulators_;
    iterator active_;
};

} // namespace

#endif
