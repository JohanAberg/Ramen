// Copyright (c) 2011 Esteban Tovagliari

#include<ramen/ui/dialogs/smooth_anim_curves_dialog.hpp>

#include<ramen/ui/user_interface.hpp>
#include<ramen/ui/main_window.hpp>

namespace ramen
{
namespace ui
{

smooth_anim_curves_dialog_impl::smooth_anim_curves_dialog_impl() : QDialog( user_interface_t::Instance().main_window())
{
	ui_.setupUi( this);
}

bool smooth_anim_curves_dialog_impl::exec( bool high_pass, float& filter_size, bool& resample)
{
	if( high_pass)
		setWindowTitle( "High Pass");
	else
		setWindowTitle( "Smooth");

	int result = QDialog::exec();
	
	if( result == QDialog::Accepted)
	{
		filter_size = ui_.filter_size_->value();
		resample = ui_.resample_->isChecked();
		return true;
	}
	
	return false;
}

} // namespace
} // namespace
