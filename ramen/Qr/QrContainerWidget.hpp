// Copyright (c) 2010 Esteban Tovagliari

#ifndef QR_CONTAINER_WIDGET_HPP
#define	QR_CONTAINER_WIDGET_HPP

#include<QStackedWidget>

class QrContainerWidget : public QStackedWidget
{
    Q_OBJECT

public:
    
    QrContainerWidget( QWidget *parent = 0);

    void setContents( QWidget *w);
    void clearContents();

private:

    QWidget *contents_;
};

#endif
