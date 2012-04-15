// Copyright (c) 2010 Esteban Tovagliari

#ifndef RAMEN_PREFERENCES_DIALOG_HPP
#define	RAMEN_PREFERENCES_DIALOG_HPP

#include<loki/Singleton.h>

#include<QDialog>

#include"ui_preferences.h"

namespace ramen
{
namespace ui
{

class preferences_dialog_impl : public QDialog
{
    Q_OBJECT
    
public:

    void exec_dialog();

private Q_SLOTS:

	void pick_disk_cache_path();

private:

    preferences_dialog_impl();

    friend struct Loki::CreateUsingNew<preferences_dialog_impl>;
	
	Ui::preferences ui_;
};

typedef Loki::SingletonHolder<preferences_dialog_impl> preferences_dialog_t;

} // namespace
} // namespace

#endif
