// Copyright (c) 2010 Esteban Tovagliari

#include<ramen/python/python.hpp>

#include<ramen/ui/dialogs/render_composition_dialog.hpp>

#include<ramen/assert.hpp>

#include<ramen/ui/user_interface.hpp>
#include<ramen/ui/main_window.hpp>

namespace ramen
{
namespace ui
{

render_composition_dialog_impl::render_composition_dialog_impl() : QDialog( user_interface_t::Instance().main_window())
{
    setWindowTitle( "Render Composition");
	ui_.setupUi( this);
}

int render_composition_dialog_impl::start_frame() const		{ return ui_.range_start_->value();}
int render_composition_dialog_impl::end_frame() const		{ return ui_.range_end_->value();}
int render_composition_dialog_impl::resolution() const		{ return ui_.resolution_popup_->currentIndex() + 1;}
int render_composition_dialog_impl::proxy_level() const		{ return ui_.proxy_popup_->currentIndex();}

bool render_composition_dialog_impl::selected_only() const	{ return ui_.output_popup_->currentIndex() == 1;}

int render_composition_dialog_impl::mblur_extra_samples() const     { return ui_.mblur_extra_samples_->value();}
float render_composition_dialog_impl::mblur_shutter_factor() const  { return ui_.mblur_shutter_factor_->value();}

void render_composition_dialog_impl::set_frame_range( int lo, int hi)
{
    ui_.range_start_->setValue( lo);
    ui_.range_end_ ->setValue( hi);
}

void render_composition_dialog_impl::set_any_output_selected( bool b)
{
    if( b)
		ui_.output_popup_->setEnabled( true);
    else
    {
		ui_.output_popup_->setCurrentIndex( 0);
		ui_.output_popup_->setEnabled( false);
    }
}

void render_composition_dialog_impl::set_mblur_settings( int samples, float factor)
{
    RAMEN_ASSERT( samples >= 0);
    RAMEN_ASSERT( factor >= 0 && factor <= 1);
    ui_.mblur_extra_samples_->setValue( samples);
	ui_.mblur_shutter_factor_->setValue( factor);
}

} // namespace
} // namespace
