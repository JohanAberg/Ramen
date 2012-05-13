// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/widgets/saturation_value_picker.hpp>

#include<QPaintEvent>
#include<QMouseEvent>
#include<QResizeEvent>
#include<QPainter>

#include<ramen/ui/widgets/color.hpp>

namespace ramen
{
namespace ui
{

saturation_value_picker_t::saturation_value_picker_t( QWidget *parent) : QWidget( parent), background_( sizeHint(), QImage::Format_RGB32)
{
    setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding);
    setFocusPolicy( Qt::StrongFocus);
    valid_background_ = false;
    hue_ = 0;
    saturation_ = 0;
    value_ = 0;
}

QSize saturation_value_picker_t::sizeHint() const { return QSize( 200, 200);}

void saturation_value_picker_t::set_hue( double h)
{
    if( hue_ != h)
    {
	hue_ = h;
	valid_background_ = false;
	update();
    }
}

void saturation_value_picker_t::set_saturation_value( double s, double v)
{
    if( v != value_ || s != saturation_)
    {
        saturation_ = s;
        value_ = v;
        saturation_value_changed( s, v);
        update();
    }
}

void saturation_value_picker_t::paintEvent( QPaintEvent *event)
{
    QPainter painter( this);

    if( event->rect().intersects(contentsRect()))
        painter.setClipRegion( event->region().intersect(contentsRect()));

    if( !valid_background_)
    {
        update_background();
        valid_background_ = true;
    }

    painter.drawImage( contentsRect().topLeft(), background_);

    int ypos = ( 1.0 - value_) * height();
    int xpos = saturation_ * width();

    QPen pen;
    pen.setWidth( 1);

    pen.setColor( QColor( 255, 255, 255));
    painter.setPen( pen);
    painter.drawLine( 0, ypos, width(), ypos);
    painter.drawLine( xpos, 0, xpos, height());

    ++xpos;
    ++ypos;
    pen.setColor( QColor( 0, 0, 0));
    painter.setPen( pen);
    painter.drawLine( 0, ypos, width(), ypos);
    painter.drawLine( xpos, 0, xpos, height());
}

void saturation_value_picker_t::mousePressEvent( QMouseEvent *event)
{
    pick_sat_val( event);
    event->accept();
}

void saturation_value_picker_t::mouseMoveEvent( QMouseEvent *event)
{
    pick_sat_val( event);
    event->accept();
}

void saturation_value_picker_t::mouseReleaseEvent( QMouseEvent *event) { event->accept();}

void saturation_value_picker_t::pick_sat_val( QMouseEvent *event)
{
    double v = 1.0 - ((double) event->y() / height());
    double s = (double) event->x() / width();

    if( v < 0) v = 0;
    if( v > 1) v = 1;

    if( s < 0) s = 0;
    if( s > 1) s = 1;

    set_saturation_value( s, v);
}

void saturation_value_picker_t::resizeEvent( QResizeEvent *event)
{
    valid_background_ = false;
    update();
}

void saturation_value_picker_t::update_background()
{
    background_ = QImage(contentsRect().size(), QImage::Format_RGB32);

    QColor col0, col1;

    for( int j = 0; j < background_.height(); ++j)
    {
		QRgb *p = reinterpret_cast<QRgb*>( background_.scanLine( j));

		double v = 1.0 - ((double) j / height());

		col0.setHsvF( hue_, 0.0, v);
		col1.setHsvF( hue_, 1.0, v);

		double r = col0.redF();
		double g = col0.greenF();
		double b = col0.blueF();

		double inc_r = ( col1.redF()   - r) / (double) width();
		double inc_g = ( col1.greenF() - g) / (double) width();
		double inc_b = ( col1.blueF()  - b) / (double) width();

		for( int i = 0; i < background_.width(); ++i)
		{
			color_t c( r, g, b);
			c.apply_gamma( 1.0 / 2.2);
			c.clamp();
			c *= 255.0;
			*p++ = qRgb( c.red(), c.green(), c.blue());
			r += inc_r;
			g += inc_g;
			b += inc_b;
		}
    }
}

} // ui
} // ramen
