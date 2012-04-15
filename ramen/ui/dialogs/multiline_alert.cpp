// Copyright (c) 2010 Esteban Tovagliari

#include<ramen/python/python.hpp>

#include<ramen/ui/dialogs/multiline_alert.hpp>

#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QButtonGroup>
#include<QDialogButtonBox>
#include<QTextEdit>

#include<ramen/ui/user_interface.hpp>
#include<ramen/ui/main_window.hpp>

namespace ramen
{
namespace ui
{

multiline_alert_impl::multiline_alert_impl() : QDialog( user_interface_t::Instance().main_window())
{
    setWindowTitle( "Alert");

    QVBoxLayout *vlayout = new QVBoxLayout( this);

	text_ = new QTextEdit();
	text_->setReadOnly( true);
	vlayout->addWidget( text_);

    QDialogButtonBox *buttonBox = new QDialogButtonBox();
    buttonBox->setOrientation( Qt::Horizontal);
    buttonBox->setStandardButtons( QDialogButtonBox::Ok);
    connect( buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    vlayout->addWidget( buttonBox);
}

void multiline_alert_impl::show_alert( const std::string& title, const std::string& text)
{
	setWindowTitle( title.c_str());
	text_->setText( text.c_str());
	int result = exec();
}

} // namespace
} // namespace
