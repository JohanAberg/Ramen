// Copyright (c) 2010 Esteban Tovagliari

#include<ramen/python/python.hpp>

#include<ramen/ui/time_controls.hpp>

#include<QHBoxLayout>
#include<QToolButton>
#include<QPushButton>
#include<QApplication>
#include<QMouseEvent>
#include<QKeyEvent>

#include<ramen/app/document.hpp>

#include<ramen/nodes/image_node.hpp>

#include<ramen/flipbook/factory.hpp>
#include<ramen/flipbook/render_flipbook.hpp>

#include<ramen/ui/user_interface.hpp>
#include<ramen/ui/inspector/inspector.hpp>
#include<ramen/ui/viewer/viewer.hpp>
#include<ramen/ui/viewer/image_view/image_viewer_strategy.hpp>

namespace ramen
{
namespace ui
{

time_controls_impl::time_controls_impl() : window_(0), stop_playing_( true)
{
    window_ = new QWidget();
    window_->resize( width(), height());
    window_->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed);
    window_->setMinimumSize( width(), height());
    window_->setMaximumSize( width(), height());
    window_->setWindowTitle( "Time Controls");
    window_->setContentsMargins( 0, 0, 0, 0);

    QHBoxLayout *layout = new QHBoxLayout( window_);
    layout->setSpacing( 0);

    start_ = new QToolButton();
    start_->setIcon( QIcon( ":icons/rewind.png"));
    start_->setFocusPolicy( Qt::NoFocus);
    connect( start_, SIGNAL( pressed()), this, SLOT( goto_start()));
    layout->addWidget( start_);

    prev_key_ = new QToolButton();
    prev_key_->setIcon( QIcon( ":icons/prev_key.png"));
    prev_key_->setFocusPolicy( Qt::NoFocus);
    prev_key_->setEnabled( false);
    connect( prev_key_, SIGNAL( pressed()), this, SLOT( prev_key()));
    layout->addWidget( prev_key_);

    prev_frame_ = new QToolButton();
    prev_frame_->setIcon( QIcon( ":icons/prev_frame.png"));
    prev_frame_->setFocusPolicy( Qt::NoFocus);
    connect( prev_frame_, SIGNAL( pressed()), this, SLOT( prev_frame()));
    layout->addWidget( prev_frame_);

    play_back_ = new QToolButton();
    play_back_->setIcon( QIcon( ":icons/play_back.png"));
    play_back_->setFocusPolicy( Qt::NoFocus);
    play_back_->setCheckable( true);
    connect( play_back_, SIGNAL( pressed()), this, SLOT( play_back()));
    layout->addWidget( play_back_);

    stop_ = new QToolButton();
    stop_->setIcon( QIcon( ":icons/stop.png"));
    stop_->setFocusPolicy( Qt::NoFocus);
    layout->addWidget( stop_);

    play_fwd_ = new QToolButton();
    play_fwd_->setIcon( QIcon( ":icons/play.png"));
    play_fwd_->setFocusPolicy( Qt::NoFocus);
    play_fwd_->setCheckable( true);
    connect( play_fwd_, SIGNAL( pressed()), this, SLOT( play_fwd()));
    layout->addWidget( play_fwd_);

    next_frame_ = new QToolButton();
    next_frame_->setIcon( QIcon( ":icons/next_frame.png"));
    next_frame_->setFocusPolicy( Qt::NoFocus);
    connect( next_frame_, SIGNAL( pressed()), this, SLOT( next_frame()));
    layout->addWidget( next_frame_);

    next_key_ = new QToolButton();
    next_key_->setIcon( QIcon( ":icons/next_key.png"));
    next_key_->setFocusPolicy( Qt::NoFocus);
    next_key_->setEnabled( false);
    connect( next_key_, SIGNAL( pressed()), this, SLOT( next_key()));
    layout->addWidget( next_key_);

    end_ = new QToolButton();
    end_->setIcon( QIcon( ":icons/fastfwd.png"));
    end_->setFocusPolicy( Qt::NoFocus);
    connect( end_, SIGNAL( pressed()), this, SLOT( goto_end()));
    layout->addWidget( end_);
	
    QSpacerItem *horizontal_spacer = new QSpacerItem(40, height(), QSizePolicy::Expanding, QSizePolicy::Minimum);
    layout->addItem( horizontal_spacer);

    flipbook_ = new QPushButton();
    flipbook_->setText( "Flipbook");
    flipbook_->setEnabled( false);
    connect( flipbook_, SIGNAL( pressed()), this, SLOT( make_flipbook()));
    layout->addWidget( flipbook_);
	
    autokey_ = new QToolButton();
    autokey_->setText( "Autokey");
    autokey_->setCheckable( true);
    autokey_->setChecked( document_t::Instance().composition().autokey());
	connect( autokey_, SIGNAL( toggled( bool)), this, SLOT( set_autokey(bool)));
    layout->addWidget( autokey_);
}

int time_controls_impl::width() const
{ 
    return user_interface_t::Instance().inspector().widget()->width();
}

int time_controls_impl::height() const { return 64;}

bool time_controls_impl::eventFilter( QObject *watched, QEvent *event)
{
    if( event->type() == QEvent::KeyPress)
    {
        QKeyEvent *key_event = dynamic_cast<QKeyEvent*>( event);

        if( key_event->key() == Qt::Key_Escape)
        {
            stop_playing();
            return true;
        }
    }

    if( event->type() == QEvent::MouseButtonPress)
    {
        stop_playing();
        return true;
    }

    return QObject::eventFilter( watched, event);
}

void time_controls_impl::update()
{
	node_t *n = document_t::Instance().composition().selected_node();
	const image_node_t *in = dynamic_cast<const image_node_t*>( n);
	flipbook_->setEnabled( in != 0);

    autokey_->blockSignals( true);
    autokey_->setChecked( document_t::Instance().composition().autokey());
    autokey_->blockSignals( false);
}

void time_controls_impl::goto_start()
{
    user_interface_t::Instance().set_frame( user_interface_t::Instance().start_frame());
}

void time_controls_impl::prev_frame()
{
    int frame = user_interface_t::Instance().frame() - 1;

    if( frame < user_interface_t::Instance().start_frame())
        frame = user_interface_t::Instance().start_frame();

    user_interface_t::Instance().set_frame( frame);
}

void time_controls_impl::prev_key() {}

void time_controls_impl::play_back()
{
    stop_playing_ = false;

    play_fwd_->blockSignals( true);
    play_fwd_->setChecked( false);
    play_fwd_->blockSignals( false);

    qApp->installEventFilter( this);

    while( !stop_playing_)
    {
        int frame = user_interface_t::Instance().frame() - 1;

        if( frame < user_interface_t::Instance().start_frame())
            frame = user_interface_t::Instance().end_frame();

        user_interface_t::Instance().set_frame( frame);
        qApp->processEvents();
    }
}

void time_controls_impl::play_fwd()
{
    stop_playing_ = false;

    play_back_->blockSignals( true);
    play_back_->setChecked( false);
    play_back_->blockSignals( false);

    qApp->installEventFilter( this);

    while( !stop_playing_)
    {
        int frame = user_interface_t::Instance().frame() + 1;

        if( frame > user_interface_t::Instance().end_frame())
            frame = user_interface_t::Instance().start_frame();

        user_interface_t::Instance().set_frame( frame);
        qApp->processEvents();
    }
}

void time_controls_impl::next_key() {}

void time_controls_impl::next_frame()
{
    int frame = user_interface_t::Instance().frame() + 1;

    if( frame > user_interface_t::Instance().end_frame())
	frame = user_interface_t::Instance().end_frame();

    user_interface_t::Instance().set_frame( frame);
}

void time_controls_impl::goto_end()
{
    user_interface_t::Instance().set_frame( user_interface_t::Instance().end_frame());
}

void time_controls_impl::stop_playing()
{
    play_fwd_->blockSignals( true);
    play_fwd_->setChecked( false);
    play_fwd_->blockSignals( false);

    play_back_->blockSignals( true);
    play_back_->setChecked( false);
    play_back_->blockSignals( false);

    qApp->removeEventFilter( this);
    stop_playing_ = true;
}

void time_controls_impl::set_autokey( bool b)
{
	document_t::Instance().composition().set_autokey( b);
}

void time_controls_impl::make_flipbook()
{
	node_t *n = document_t::Instance().composition().selected_node();
	image_node_t *inode = dynamic_cast<image_node_t*>( n);
	RAMEN_ASSERT( inode);

    int frame_rate = document_t::Instance().composition().frame_rate();
	int start = document_t::Instance().composition().start_frame();
    int end   = document_t::Instance().composition().end_frame();
	int subsample = 1;
	int proxy_level = 0;
	float mb_shutter_factor = 0;
	
	const viewer::image_viewer_strategy_t *s = 0;
	if( s = dynamic_cast<const viewer::image_viewer_strategy_t*>( &viewer_t::Instance().current_viewer()))
	{
		subsample = s->subsample();
		
		if( s->mblur_active())
			mb_shutter_factor = 1;
	}

    std::string display_device = viewer_t::Instance().display_device();
	std::string display_transform = viewer_t::Instance().display_transform();

	flipbook::flipbook_t *flip = flipbook::factory_t::Instance().create( frame_rate, display_device, display_transform);

	if( flip)
	{
		if( flipbook::render_flipbook( flip, inode, start, end, proxy_level, subsample, 0, mb_shutter_factor))
			flip->play();
	}
}

} // namespace
} // namespace
