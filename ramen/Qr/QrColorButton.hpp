// Copyright (c) 2010 Esteban Tovagliari

#ifndef RAMEN_QRCOLORBUTTON_HPP
#define	RAMEN_QRCOLORBUTTON_HPP

#include<QPushButton>

#include<ramen/Qr/QrColor.hpp>

class QrColorButton : public QPushButton
{
    Q_OBJECT

public:

    QrColorButton( QWidget *parent = 0);

    const QrColor& value() const { return value_;}

public Q_SLOTS:

    void setValue( const QrColor& c);

    void setRed( double x);
    void setGreen( double x);
    void setBlue( double x);

Q_SIGNALS:

    void valueChanged( const QrColor&);

private:

    void updateButtonColor();

    double clamp( double x) const
    {
        if( x < 0)
            return 0;

        if( x > 1)
            return 1;

        return x;
    }

    QrColor value_;
};

#endif
