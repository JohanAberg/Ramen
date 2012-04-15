// Copyright (c) 2010 Esteban Tovagliari

#ifndef RAMEN_UI_WIDGETS_IMAGE_FORMAT_HPP
#define RAMEN_UI_WIDGETS_IMAGE_FORMAT_HPP

#include<QWidget>

#include<ramen/image/format.hpp>

class QComboBox;
class QSpinBox;
class QDoubleSpinBox;

namespace ramen
{
namespace ui
{

class image_format_widget_t : public QWidget
{
    Q_OBJECT

public:

	image_format_widget_t( QWidget *parent = 0);

	image::format_t value() const;
	void set_value( const image::format_t& format);

private Q_SLOTS:

    void width_changed( int x);
    void height_changed( int x);
    void aspect_changed( double x);
    void preset_picked( int x);

private:

	void update_presets_menu();

    QComboBox *format_combo_;
	QSpinBox *format_width_, *format_height_;
	QDoubleSpinBox *format_aspect_;
};

} // ui
} // ramen

#endif
