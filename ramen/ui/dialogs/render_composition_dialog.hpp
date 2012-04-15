// Copyright (c) 2010 Esteban Tovagliari

#ifndef RAMEN_RENDER_COMPOSITION_DIALOG_HPP
#define	RAMEN_RENDER_COMPOSITION_DIALOG_HPP

#include<loki/Singleton.h>

#include<QDialog>

#include"ui_render_composition.h"

namespace ramen
{
namespace ui
{

class render_composition_dialog_impl : public QDialog
{
    Q_OBJECT
    
public:

    int start_frame() const;
    int end_frame() const;
	int proxy_level() const;	
    int resolution() const;
	
    bool selected_only() const;
    int mblur_extra_samples() const;
    float mblur_shutter_factor() const;

    void set_frame_range( int lo, int hi);
    void set_any_output_selected( bool b);
    void set_mblur_settings( int samples, float factor);

private:

    friend struct Loki::CreateUsingNew<render_composition_dialog_impl>;

    render_composition_dialog_impl();

	Ui::render_composition ui_;
};

typedef Loki::SingletonHolder<render_composition_dialog_impl> render_composition_dialog_t;

} // namespace
} // namespace

#endif
