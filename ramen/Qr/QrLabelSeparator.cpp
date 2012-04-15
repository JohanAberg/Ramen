// Copyright (c) 2010 Esteban Tovagliari

#include<ramen/Qr/QrLabelSeparator.hpp>

#include<QFrame>
#include<QHBoxLayout>

QrLabelSeparator::QrLabelSeparator( QWidget *parent) : QWidget( parent)
{
    label_ = new QLabel();
    init();
}

QrLabelSeparator::QrLabelSeparator( const QString& text, QWidget *parent) : QWidget( parent)
{
    label_ = new QLabel( text);
    init();
}

void QrLabelSeparator::init()
{
    QHBoxLayout *hlayout = new QHBoxLayout( this);

    QSizePolicy sizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth( label_->sizePolicy().hasHeightForWidth());
    label_->setSizePolicy( sizePolicy);
    hlayout->addWidget( label_);

    QFrame *line = new QFrame();
    line->setFrameShape( QFrame::HLine);
    line->setFrameShadow( QFrame::Sunken);
    hlayout->addWidget( line);
}
