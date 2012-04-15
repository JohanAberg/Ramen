// Copyright (c) 2010 Esteban Tovagliari

#ifndef RAMEN_UI_INSPECTOR_HPP
#define	RAMEN_UI_INSPECTOR_HPP

#include<boost/noncopyable.hpp>

#include<loki/Singleton.h>

#include<ramen/Qr/QrContainerWidget.hpp>

#include<ramen/nodes/node_fwd.hpp>

#include<ramen/ui/inspector/panel_factory.hpp>

#include<ramen/ui/widgets/line_edit.hpp>

class QScrollArea;
class QPushButton;

namespace ramen
{

namespace ui
{

class panel_t;

class inspector_impl : public QObject, boost::noncopyable
{
    Q_OBJECT

public:

    // metrics
    int left_margin() const;
    int width() const;

    void edit_node( node_t *n);

    void update();

    QWidget *widget() { return window_;}

	panel_factory_t& panel_factory() { return factory_;}

private Q_SLOTS:

    void rename_node();
	void show_help();
	
private:

    friend struct Loki::CreateUsingNew<inspector_impl>;

    inspector_impl();

    void create_header();

    void recreate_panel( node_t *n);

    void update_header_widgets();

    // metrics
    mutable int left_margin_;
    mutable int width_;

    QWidget *window_;
    QScrollArea *scroll_;

    QWidget *header_;
    ui::line_edit_t *name_edit_;
	QPushButton *help_;

    QrContainerWidget *view_;

	panel_factory_t factory_;
    panel_factory_t::iterator current_;
};

typedef Loki::SingletonHolder<inspector_impl> inspector_t;

} // namespace
} // namespace

#endif
