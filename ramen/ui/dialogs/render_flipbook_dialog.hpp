// Copyright (c) 2010 Esteban Tovagliari

#ifndef RAMEN_RENDER_FLIPBOOK_DIALOG_HPP
#define	RAMEN_RENDER_FLIPBOOK_DIALOG_HPP

#include<vector>
#include<string>

#include<loki/Singleton.h>

#include<QDialog>

#include"ui_render_flipbook.h"

namespace ramen
{
namespace ui
{

class render_flipbook_dialog_impl : public QDialog
{
    Q_OBJECT
    
public:

    int start_frame() const;
    int end_frame() const;
	int proxy_level() const;
    int resolution() const;

	std::string flipbook() const;
	
    int mblur_extra_samples() const;
    float mblur_shutter_factor() const;
	
    void set_frame_range( int lo, int hi);

    const std::string& display_device() const;
    const std::string& display_transform() const;

private Q_SLOTS:
	
	void change_display_device( int index);
	void change_display_transform( int index);

private:

    friend struct Loki::CreateUsingNew<render_flipbook_dialog_impl>;

    render_flipbook_dialog_impl();

    void get_display_devices();
    void get_display_transforms();
	
	Ui::render_flipbook ui_;
	
    std::string display_device_;
    std::string display_transform_;
    std::vector<std::string> display_devices_;
    std::vector<std::string> display_transforms_;	
};

typedef Loki::SingletonHolder<render_flipbook_dialog_impl> render_flipbook_dialog_t;

} // namespace
} // namespace

#endif
