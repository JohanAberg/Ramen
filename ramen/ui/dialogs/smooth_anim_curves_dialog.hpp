// Copyright (c) 2011 Esteban Tovagliari

#ifndef RAMEN_SMOOTH_ANIM_CURVES_DIALOG_HPP
#define	RAMEN_SMOOTH_ANIM_CURVES_DIALOG_HPP

#include<ramen/python/python.hpp>

#include<loki/Singleton.h>

#include<QDialog>

#include"ui_smooth_anim_curves.h"

namespace ramen
{
namespace ui
{
	
class smooth_anim_curves_dialog_impl : public QDialog
{
    Q_OBJECT
    
public:

	bool exec( bool high_pass, float& filter_size, bool& resample);

private:
	
    friend struct Loki::CreateUsingNew<smooth_anim_curves_dialog_impl>;

    smooth_anim_curves_dialog_impl();

	Ui::smooth_anim_curves ui_;
};

typedef Loki::SingletonHolder<smooth_anim_curves_dialog_impl> smooth_anim_curves_dialog_t;

} // namespace
} // namespace

#endif
