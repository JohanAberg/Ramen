// Copyright (c) 2011 Esteban Tovagliari

#ifndef RAMEN_UI_COMPOSITION_VIEW_TOOLBAR_HPP
#define	RAMEN_UI_COMPOSITION_VIEW_TOOLBAR_HPP

#include<QWidget>

namespace ramen
{
namespace ui
{

class composition_view_t;

class composition_view_toolbar_t : public QWidget
{
    Q_OBJECT

public:

    composition_view_toolbar_t( composition_view_t *comp_view);
	
private:

	int toolbar_height() const;

    composition_view_t *comp_view_;
};

} // namespace
} // namespace

#endif
