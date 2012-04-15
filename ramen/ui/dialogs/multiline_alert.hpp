// Copyright (c) 2010 Esteban Tovagliari

#ifndef RAMEN_MULTILINE_ALERT_HPP
#define	RAMEN_MULTILINE_ALERT_HPP

#include<string>

#include<loki/Singleton.h>

#include<QDialog>

class QTextEdit;

namespace ramen
{
namespace ui
{

class multiline_alert_impl : public QDialog
{
    Q_OBJECT

public:

    multiline_alert_impl();

    void show_alert( const std::string& title, const std::string& text);

private:

    friend struct Loki::CreateUsingNew<multiline_alert_impl>;

	QTextEdit *text_;
};

typedef Loki::SingletonHolder<multiline_alert_impl> multiline_alert_t;

} // namespace
} // namespace

#endif
