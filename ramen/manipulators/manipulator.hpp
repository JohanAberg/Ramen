// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_MANIPULATOR_HPP
#define RAMEN_MANIPULATOR_HPP

#include<ramen/config.hpp>

#include<string>

#include<boost/noncopyable.hpp>

#include<OpenEXR/ImathColor.h>

#include<ramen/assert.hpp>

#include<ramen/manipulators/manipulable_fwd.hpp>

#include<ramen/ui/events.hpp>

namespace ramen
{

class RAMEN_API manipulator_t : boost::noncopyable
{
public:

    manipulator_t();
    virtual ~manipulator_t() {}

    const manipulable_t *parent() const { return parent_;}
    manipulable_t *parent()             { return parent_;}
    void set_parent( manipulable_t *p)  { parent_ = p;}

	bool visible() const		{ return visible_;}
	void set_visible( bool b)	{ visible_ = b;}

	bool active() const			{ return active_;}
	void set_active( bool b)	{ active_ = b;}
	
    void draw_overlay( const ui::paint_event_t& event) const;

    bool key_press_event( const ui::key_press_event_t& event);
    void key_release_event( const ui::key_release_event_t& event);

    void mouse_enter_event( const ui::mouse_enter_event_t& event);
    void mouse_leave_event( const ui::mouse_leave_event_t& event);

    bool mouse_press_event( const ui::mouse_press_event_t& event);
    void mouse_move_event( const ui::mouse_move_event_t& event);
    void mouse_drag_event( const ui::mouse_drag_event_t& event);
    void mouse_release_event( const ui::mouse_release_event_t& event);
	
	static float default_control_point_size();
	static float default_line_width();
	
	static const Imath::Color3c& default_color();
	static const Imath::Color3c& selected_color();

private:

    virtual void do_draw_overlay( const ui::paint_event_t& event) const;

    virtual bool do_key_press_event( const ui::key_press_event_t& event);
    virtual void do_key_release_event( const ui::key_release_event_t& event);

    virtual void do_mouse_enter_event( const ui::mouse_enter_event_t& event);
    virtual void do_mouse_leave_event( const ui::mouse_leave_event_t& event);

    virtual bool do_mouse_press_event( const ui::mouse_press_event_t& event);
    virtual void do_mouse_move_event( const ui::mouse_move_event_t& event);
    virtual void do_mouse_drag_event( const ui::mouse_drag_event_t& event);
    virtual void do_mouse_release_event( const ui::mouse_release_event_t& event);
	
    manipulable_t *parent_;
	bool visible_;
	bool active_;

public: // These are for the python bindings, do not use.

	void __do_draw_overlay( const ui::paint_event_t& event) const			{ manipulator_t::do_draw_overlay( event);}
	bool __do_key_press_event( const ui::key_press_event_t& event)			{ return manipulator_t::do_key_press_event( event);}
	void __do_key_release_event( const ui::key_release_event_t& event)		{ manipulator_t::do_key_release_event( event);}
	void __do_mouse_enter_event( const ui::mouse_enter_event_t& event)		{ manipulator_t::do_mouse_enter_event( event);}
	void __do_mouse_leave_event( const ui::mouse_leave_event_t& event)		{ manipulator_t::do_mouse_leave_event( event);}
	bool __do_mouse_press_event( const ui::mouse_press_event_t& event)		{ return manipulator_t::do_mouse_press_event( event);}
	void __do_mouse_move_event( const ui::mouse_move_event_t& event)		{ manipulator_t::do_mouse_move_event( event);}
	void __do_mouse_drag_event( const ui::mouse_drag_event_t& event)		{ manipulator_t::do_mouse_drag_event( event);}
	void __do_mouse_release_event( const ui::mouse_release_event_t& event)	{ manipulator_t::do_mouse_release_event( event);}
};

} // namespace

#endif
