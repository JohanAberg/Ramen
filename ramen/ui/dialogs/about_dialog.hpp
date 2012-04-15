// Copyright (c) 2011 Esteban Tovagliari

#ifndef RAMEN_ABOUT_DIALOG_HPP
#define	RAMEN_ABOUT_DIALOG_HPP

#include<loki/Singleton.h>

#include<QDialog>

#include"ui_about.h"

namespace ramen
{
namespace ui
{

class about_dialog_impl : public QDialog
{
    Q_OBJECT
    
public:

private:

    friend struct Loki::CreateUsingNew<about_dialog_impl>;

    about_dialog_impl();

	Ui::about ui_;
};

typedef Loki::SingletonHolder<about_dialog_impl> about_dialog_t;

} // namespace
} // namespace

#endif
