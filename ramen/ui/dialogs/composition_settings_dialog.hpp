// Copyright (c) 2010 Esteban Tovagliari

#ifndef RAMEN_COMP_SETTINGS_DIALOG_HPP
#define	RAMEN_COMP_SETTINGS_DIALOG_HPP

#include<loki/Singleton.h>

#include<QDialog>

#include"ui_composition_settings.h"

namespace ramen
{
namespace ui
{

class image_format_widget_t;

class composition_settings_dialog_impl : public QDialog
{
    Q_OBJECT

public:

    void exec_dialog();

private:

    friend struct Loki::CreateUsingNew<composition_settings_dialog_impl>;

    composition_settings_dialog_impl();

	Ui::composition_settings ui_;
};

typedef Loki::SingletonHolder<composition_settings_dialog_impl> composition_settings_dialog_t;

} // namespace
} // namespace

#endif
