// Copyright (c) 2011 Esteban Tovagliari

#ifndef RAMEN_UI_PY_CONSOLE_HPP
#define	RAMEN_UI_PY_CONSOLE_HPP

#include<boost/noncopyable.hpp>

#include<loki/Singleton.h>

#include<QObject>

class QWidget;
class QPlainTextEdit;

namespace ramen
{
namespace ui
{
namespace python
{

class console_impl : public QObject, boost::noncopyable
{
    Q_OBJECT

public:

    QWidget *widget() { return window_;}

    QPlainTextEdit *output_area()	{ return output_;}

    void write( const std::string& str);

public Q_SLOTS:

    void clear();

private:

    console_impl();
    ~console_impl();

    friend struct Loki::CreateUsingNew<console_impl>;

    QWidget *window_;
    QPlainTextEdit *output_;
};

typedef Loki::SingletonHolder<console_impl> console_t;

} // namespace
} // namespace
} // namespace

#endif
