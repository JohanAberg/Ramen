// Copyright (c) 2011 Esteban Tovagliari

#ifndef RAMEN_UI_PY_EDITOR_HPP
#define	RAMEN_UI_PY_EDITOR_HPP

#include<boost/noncopyable.hpp>

#include<loki/Singleton.h>

#include<QObject>

class QPoint;
class QWidget;
class QPlainTextEdit;

namespace ramen
{
namespace ui
{
namespace python
{

class editor_impl : public QObject, boost::noncopyable
{
    Q_OBJECT

public:

    QWidget *widget() { return window_;}

    QPlainTextEdit *edit_area()	{ return edit_;}

private Q_SLOTS:

    void run();
	void show_context_menu( const QPoint& p);

private:

    editor_impl();
    ~editor_impl();

    friend struct Loki::CreateUsingNew<editor_impl>;

    QWidget *window_;
    QPlainTextEdit *edit_;
};

typedef Loki::SingletonHolder<editor_impl> editor_t;

} // namespace
} // namespace
} // namespace

#endif
