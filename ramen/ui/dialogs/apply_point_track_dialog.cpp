// Copyright (c) 2011 Esteban Tovagliari

#include<ramen/ui/dialogs/apply_point_track_dialog.hpp>

#include<vector>

#include<boost/foreach.hpp>

#include<QMessageBox>

#include<ramen/assert.hpp>

#include<ramen/app/document.hpp>

#include<ramen/nodes/image/track/tracker_node.hpp>

#include<ramen/ui/user_interface.hpp>
#include<ramen/ui/main_window.hpp>

namespace ramen
{
namespace ui
{

apply_point_track_dialog_impl::apply_point_track_dialog_impl() : QDialog( user_interface_t::Instance().main_window())
{
    setWindowTitle( "Apply Track");
	ui_.setupUi( this);
}

const image::tracker_node_t *apply_point_track_dialog_impl::exec( int& track_num)
{
	std::vector<const image::tracker_node_t*> trackers;
	ui_.tracker_combo_->clear();
	
	BOOST_FOREACH( const node_ptr_t& n, document_t::Instance().composition().nodes())
	{
		const image::tracker_node_t *t = dynamic_cast<const image::tracker_node_t*>( n.get());
		
		if( t)
		{
			if( !t->ignored())
			{
				ui_.tracker_combo_->addItem( QString::fromStdString( n->name()));
				trackers.push_back( t);
			}
		}
	}

	if( ui_.tracker_combo_->count() == 0)
	{
        QMessageBox::warning( ui::user_interface_t::Instance().main_window(), "Ramen", 
							  "No active tracker nodes in composition");
		return 0;
	}

	int result = QDialog::exec();
	
	if( result == QDialog::Rejected)
		return 0;

	track_num = ui_.num_combo_->currentIndex();
	return trackers[ ui_.tracker_combo_->currentIndex()];
}

} // namespace
} // namespace
