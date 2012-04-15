// Copyright (c) 2011 Esteban Tovagliari

#ifndef RAMEN_APPLY_CORNER_TRACK_DIALOG_HPP
#define	RAMEN_APPLY_CORNER_TRACK_DIALOG_HPP

#include<ramen/python/python.hpp>

#include<loki/Singleton.h>

#include<QDialog>

#include"ui_apply_corner_track.h"

#include<ramen/nodes/image/track/tracker_node_fwd.hpp>

namespace ramen
{
namespace ui
{
	
class apply_corner_track_dialog_impl : public QDialog
{
    Q_OBJECT
    
public:

	const image::tracker_node_t *exec();
	
private:
	
    friend struct Loki::CreateUsingNew<apply_corner_track_dialog_impl>;

    apply_corner_track_dialog_impl();

	Ui::apply_corner_track ui_;
};

typedef Loki::SingletonHolder<apply_corner_track_dialog_impl> apply_corner_track_dialog_t;

} // namespace
} // namespace

#endif
