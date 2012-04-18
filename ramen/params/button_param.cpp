// Copyright (c) 2010 Esteban Tovagliari

#include<ramen/params/button_param.hpp>

#include<ramen/params/param_set.hpp>

#include<QPushButton>

#include<ramen/ui/user_interface.hpp>
#include<ramen/ui/inspector/inspector.hpp>

namespace ramen
{

button_param_t::button_param_t( const std::string& name) : param_t( name) { set_static( true);}
button_param_t::button_param_t( const button_param_t& other) : param_t( other)
{
	#ifndef RAMEN_NO_GUI
		button_ = 0;
	#endif
}

void button_param_t::do_enable_widgets( bool e)
{
	#ifndef RAMEN_NO_GUI
	    if( button_)
	        button_->setEnabled( e);
	#endif
}

#ifndef RAMEN_NO_GUI
	QWidget *button_param_t::do_create_widgets()
	{
		QWidget *top = new QWidget();
	
		button_ = new QPushButton( top);
		button_->setText( name().c_str());
		connect( button_, SIGNAL( pressed()), this, SLOT( button_pressed()));
	
		QSize s = button_->sizeHint();
	
		button_->move( ui::user_interface_t::Instance().inspector().left_margin(), 0);
		button_->resize( s.width(), s.height());
		button_->setEnabled( enabled());
		button_->setToolTip( id().c_str());
		top->setMinimumSize( ui::user_interface_t::Instance().inspector().width(), s.height());
		top->setMaximumSize( ui::user_interface_t::Instance().inspector().width(), s.height());
		top->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed);
		return top;
	}

	void button_param_t::button_pressed() { emit_param_changed( user_edited);}
#endif

} // namespace
