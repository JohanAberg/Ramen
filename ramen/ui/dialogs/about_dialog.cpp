// Copyright (c) 2011 Esteban Tovagliari

#include<ramen/python/python.hpp>

#include<ramen/ui/dialogs/about_dialog.hpp>

#include<ramen/ui/user_interface.hpp>
#include<ramen/ui/main_window.hpp>

namespace ramen
{
namespace ui
{

about_dialog_impl::about_dialog_impl() : QDialog( user_interface_t::Instance().main_window())
{
	ui_.setupUi( this);
	// setup version string here.
}

} // namespace
} // namespace
