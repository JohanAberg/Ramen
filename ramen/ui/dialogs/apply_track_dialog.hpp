// Copyright (c) 2011 Esteban Tovagliari

#ifndef RAMEN_APPLY_TRACK_DIALOG_HPP
#define	RAMEN_APPLY_TRACK_DIALOG_HPP

#include<ramen/python/python.hpp>

#include<loki/Singleton.h>

#include<QDialog>

#include"ui_apply_track.h"

#include<ramen/nodes/image/track/tracker_node_fwd.hpp>

namespace ramen
{
namespace ui
{
	
class apply_track_dialog_impl : public QDialog
{
    Q_OBJECT
    
public:

	const image::tracker_node_t *exec( image::apply_track_mode& mode, image::apply_track_use& use);
	
private:
	
    friend struct Loki::CreateUsingNew<apply_track_dialog_impl>;

    apply_track_dialog_impl();

	Ui::apply_track ui_;
};

typedef Loki::SingletonHolder<apply_track_dialog_impl> apply_track_dialog_t;

} // namespace
} // namespace

#endif
