// Copyright (c) 2010 Esteban Tovagliari

#include<ramen/ui/widgets/eyedropper_button.hpp>

#include<QApplication>
#include<QMouseEvent>
#include<QKeyEvent>

#include<ramen/Qr/QrColorButton.hpp>

#include<ramen/ui/viewer/viewer_context.hpp>

#include<iostream>

namespace ramen
{
namespace ui
{

eyedropper_button_t::eyedropper_button_t( QWidget *parent) : QPushButton( QIcon( ":icons/eyedropper.png"), "", parent)
{
    setFocusPolicy( Qt::NoFocus);
    setCheckable( true);
    setChecked( false);
    connect( this, SIGNAL( toggled( bool)), this, SLOT( tool_selected( bool)));
}

bool eyedropper_button_t::eventFilter( QObject *watched, QEvent *event)
{
    if( event->type() == QEvent::KeyPress)
    {
		QKeyEvent *key_event = dynamic_cast<QKeyEvent*>( event);

		if( key_event->key() == Qt::Key_Escape)
		{
		    qApp->removeEventFilter( this);
		    setChecked( false);
		    QApplication::restoreOverrideCursor();
		}
    }

    if( event->type() == QEvent::MouseButtonPress)
    {
		QMouseEvent *mouse_event = dynamic_cast<QMouseEvent*>( event);

		if( const ui::viewer::viewer_context_t *view = dynamic_cast<const ui::viewer::viewer_context_t*>( watched))
		{
		    QPoint local_pos = view->mapFromGlobal( mouse_event->globalPos());
			Imath::Color4f col = view->color_at( local_pos.x(), local_pos.y());
		    value_ = QrColor( col.r, col.g, col.b, col.a);
		}
		else
		{
			if( const QrColorButton *but = dynamic_cast<const QrColorButton*>( watched))
			{
				QrColor col = but->value();
				col.applyGamma( 2.2);
				value_ = col;
			}
			else
			{
			    // TODO: can we do better here?
			    value_ = QrColor( 0, 0, 0);
			}
		}

		qApp->removeEventFilter( this);
		setChecked( false);
		QApplication::restoreOverrideCursor();
		color_picked( value_);
		return true;
    }

    return QObject::eventFilter( watched, event);
}

void eyedropper_button_t::tool_selected( bool b)
{
    if( b)
    {
		QApplication::setOverrideCursor( QCursor( Qt::CrossCursor));
		qApp->installEventFilter( this);
    }
}

} // namespace
} // namespace
