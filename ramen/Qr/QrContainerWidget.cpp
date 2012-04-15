// Copyright (c) 2010 Esteban Tovagliari

#include<ramen/Qr/QrContainerWidget.hpp>

QrContainerWidget::QrContainerWidget( QWidget *parent) : QStackedWidget( parent), contents_(0) {}

void QrContainerWidget::setContents( QWidget *w)
{
    clearContents();

    if( w)
    {
        addWidget( w);
		setCurrentIndex( 0);
		contents_ = w;
    }
}

void QrContainerWidget::clearContents()
{
    if( contents_)
    {
		removeWidget( contents_);
		contents_ = 0;
    }
}
