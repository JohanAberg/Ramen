// Copyright (c) 2010 Esteban Tovagliari

#include<ramen/python/python.hpp>

#include<ramen/ui/dialogs/preferences_dialog.hpp>

#include<ramen/app/application.hpp>

#include<ramen/ui/user_interface.hpp>
#include<ramen/ui/main_window.hpp>

#include<ramen/flipbook/factory.hpp>

namespace ramen
{
namespace ui
{

preferences_dialog_impl::preferences_dialog_impl() : QDialog( user_interface_t::Instance().main_window())
{
    setWindowTitle( "Preferences");
	ui_.setupUi( this);
	
	// connect signals
	connect( ui_.ram_slider_, SIGNAL( valueChanged(int)), ui_.ram_spinbox_, SLOT( setValue(int)));
	connect( ui_.ram_spinbox_, SIGNAL( valueChanged(int)), ui_.ram_slider_, SLOT( setValue(int)));

	connect( ui_.disk_slider_, SIGNAL( valueChanged(int)), ui_.disk_spinbox_, SLOT( setValue(int)));
	connect( ui_.disk_spinbox_, SIGNAL( valueChanged(int)), ui_.disk_slider_, SLOT( setValue(int)));

	// init flipbook list
	{
		int index = 0;
		QStringList slist;

		std::string default_flipbook = app().preferences().default_flipbook();
		
		for( int i = 0; i < flipbook::factory_t::Instance().flipbooks().size(); ++i)
		{
			std::string fname = flipbook::factory_t::Instance().flipbooks()[i].first;
			slist << fname.c_str();
			
			if( fname == default_flipbook)
				index = i;
		}
		
		ui_.flipbook_combo_->addItems( slist);
		ui_.flipbook_combo_->setCurrentIndex( index);
	}	

	// disk cache
	ui_.disk_cache_path_->setText( "/tmp/ramen/cache");
}

void preferences_dialog_impl::exec_dialog()
{
	// update widgets value here...
	ui_.default_format_->set_value( app().preferences().default_format());
	ui_.frame_rate_->setValue( app().preferences().frame_rate());
	ui_.ram_slider_->setValue( app().preferences().max_image_memory());
	ui_.ram_spinbox_->setValue( app().preferences().max_image_memory());

    int result = exec();

    if( result == QDialog::Accepted)
    {
		app().preferences().set_max_image_memory( ui_.ram_spinbox_->value());
		app().preferences().set_default_format( ui_.default_format_->value());
		app().preferences().set_frame_rate( ui_.frame_rate_->value());
		app().preferences().set_default_flipbook( ui_.flipbook_combo_->currentText().toStdString());
		app().preferences().save();
    }
}

void preferences_dialog_impl::pick_disk_cache_path() {}

} // namespace
} // namespace
