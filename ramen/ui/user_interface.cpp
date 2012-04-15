// Copyright (c) 2010 Esteban Tovagliari

#include<ramen/python/python.hpp>

#include<ramen/ui/user_interface.hpp>

#include<utility>
#include<iostream>

#include<boost/foreach.hpp>
#include<boost/thread.hpp>
#include<boost/filesystem/operations.hpp>

#include<QApplication>
#include<QSplashScreen>
#include<QMessageBox>
#include<QFile>
#include<QFileDialog>
#include<QGridLayout>
#include<QHBoxLayout>
#include<QCheckBox>
#include<QKeyEvent>
#include<QPlastiqueStyle>

#include<ramen/assert.hpp>

#include<ramen/app/application.hpp>
#include<ramen/app/document.hpp>
#include<ramen/app/preferences.hpp>

#include<ramen/undo/stack.hpp>

#include<ramen/movieio/factory.hpp>

#include<ramen/render/image_node_renderer.hpp>

#include<ramen/ui/main_window.hpp>
#include<ramen/ui/palette.hpp>

#include<ramen/ui/inspector/inspector.hpp>
#include<ramen/ui/viewer/viewer.hpp>
#include<ramen/ui/anim/anim_editor.hpp>

#include<ramen/ui/dialogs/multiline_alert.hpp>
#include<ramen/ui/dialogs/render_composition_dialog.hpp>
#include<ramen/ui/dialogs/render_flipbook_dialog.hpp>

#include<ramen/ui/widgets/time_slider.hpp>

#include<ramen/serialization/yaml_iarchive.hpp>
#include<ramen/serialization/yaml_oarchive.hpp>

namespace ramen
{
namespace ui
{

user_interface_impl::user_interface_impl() : QObject()
{
	// RAMEN_ASSERT( !app().command_line());
}

void user_interface_impl::init()
{
	active_ = 0;
	context_ = 0;
	rendering_ = false;
	quitting_ = false;
	cancelled_ = false;
	interacting_ = false;
	event_filter_installed_ = false;

	init_ui_style();
	
    image_types_str_ = "Image Files (";

	    BOOST_FOREACH( const std::string& ext, movieio::factory_t::Instance().extensions())
	    {
			image_types_str_.append( "*.");
			image_types_str_.append( ext.c_str());
			image_types_str_.append( " ");
	    }

	image_types_str_.append( ")");

    viewer_t::Instance().init();
    window_ = new main_window_t();

    // force creation of singletons to avoid a crash at exit
    viewer_t::Instance();
    inspector_t::Instance();
    anim_editor_t::Instance();

    create_new_document();

    // restore the last saved window state
	restore_window_state();
}

void user_interface_impl::init_ui_style()
{
	QPlastiqueStyle *style = new QPlastiqueStyle();
	qApp->setStyle( style);

	// init palette
	palette_t::Instance();
}

void user_interface_impl::restore_window_state()
{
    boost::filesystem::path p = app().system().preferences_path() / "wstate.ui";
	{
	    QFile file(filesystem::file_cstring( p));

	    if( file.open( QIODevice::ReadOnly))
	    {
			QByteArray window_state = file.readAll();
			window_->restoreState( window_state);
			return;
	    }
	}
	
	// try a default state here...
}

void user_interface_impl::show() { window_->show();}

int user_interface_impl::run( const boost::filesystem::path& p)
{ 
	if( !p.empty())
		open_document( p);

	return qApp->exec();
}

void user_interface_impl::quit()
{
    undo::stack_t::Instance().clear();
    Loki::DeletableSingleton<document_impl>::GracefulDelete();

    quitting_ = true;

    QByteArray window_state = window_->saveState();
    boost::filesystem::path p = app().system().preferences_path() / "wstate.ui";
    QFile file( filesystem::file_cstring( p));

    if( file.open( QIODevice::WriteOnly))
	file.write( window_state);

    qApp->quit();
}

void user_interface_impl::create_new_document()
{
    set_active_node( 0);
    set_context_node( 0);
	anim_editor_t::Instance().clear_all();
	
	app().create_new_document();

    document_t::Instance().composition().attach_add_observer( boost::bind( &user_interface_impl::node_added, this, _1));
    document_t::Instance().composition().attach_release_observer( boost::bind( &user_interface_impl::node_released, this, _1));

    render_composition_dialog_t::Instance().set_frame_range( document_t::Instance().composition().start_frame(),
																document_t::Instance().composition().end_frame());

    render_composition_dialog_t::Instance().set_mblur_settings( 0, 1);

    render_flipbook_dialog_t::Instance().set_frame_range( document_t::Instance().composition().start_frame(),
															document_t::Instance().composition().end_frame());
    
    update();
}

void user_interface_impl::open_document( const boost::filesystem::path& p)
{
    create_new_document();
    boost::filesystem::ifstream ifs( p, serialization::yaml_iarchive_t::file_open_mode());

    if( !ifs.is_open() || !ifs.good())
    {
        error( std::string( "Couldn't open input file ") + filesystem::file_string( p));
		update();
        return;
    }

	std::auto_ptr<serialization::yaml_iarchive_t> in;
	
	try
	{
		in.reset( new serialization::yaml_iarchive_t( ifs));
		
		if( !in->read_composition_header())
		{
			create_new_document();
			error( std::string( "Couldn't open input file ") + filesystem::file_string( p));
			update();
			return;
		}
		
		document_t::Instance().set_file( p);
		document_t::Instance().load( *in);
		main_window()->update_recent_files_menu( p);
	}
	catch( std::exception& e)
	{
		create_new_document();
        error( std::string( "Couldn't open input file ") + filesystem::file_string( p));
		update();
		return;
	}

	// read here ui info

	// update the dialogs
    render_composition_dialog_t::Instance().set_frame_range( document_t::Instance().composition().start_frame(),
                                                    document_t::Instance().composition().end_frame());

    render_composition_dialog_t::Instance().set_mblur_settings( 0, 1);

    render_flipbook_dialog_t::Instance().set_frame_range( document_t::Instance().composition().start_frame(),
															document_t::Instance().composition().end_frame());
	
    update();
	std::string err = in->errors();
	
	if( !err.empty())
		multiline_alert_t::Instance().show_alert( "Errors during file open", err);
}

bool user_interface_impl::save_document()
{
    RAMEN_ASSERT( document_t::Instance().has_file());

	try
	{
		serialization::yaml_oarchive_t out;
		out.write_composition_header();
		document_t::Instance().save( out);
		write_ui_state( out);

		out.write_to_file( document_t::Instance().file());
		document_t::Instance().set_dirty( false);
	}
	catch( std::exception& e)
	{
		error( std::string( "Couldn't save file. Exception, what = ") + e.what());
		return false;
	}
	
	return true;
}

void user_interface_impl::set_active_node( node_t *n)
{
    if( active_ != n)
    {
        if( active_)
            active_->end_active();

        active_ = n;

        if( active_)
            active_->begin_active();

        inspector_t::Instance().edit_node( n);
        viewer_t::Instance().set_active_node( n);
        anim_editor_t::Instance().set_active_node( n);
    }
}

void user_interface_impl::set_context_node( node_t *n)
{
    if( context_ != n)
    {
        if( context_)
            context_->end_context();

        context_ = n;

        if( context_)
            context_->begin_context();

        viewer_t::Instance().set_context_node( n);
    }
}

void user_interface_impl::node_added( node_t *n)
{
    viewer_t::Instance().node_added( n);
}

void user_interface_impl::node_released( node_t *n)
{
    if( n == active_)
    {
        set_active_node( 0);
        update();
    }

    if( n == context_)
    {
        set_context_node( 0);
        update();
    }

    viewer_t::Instance().node_released( n);
}

void user_interface_impl::update()
{
    if( !quitting_)
	{
        window_->update();
		update_anim_editors();
	}
}

void user_interface_impl::begin_interaction()
{
	document_t::Instance().composition().begin_interaction();
	viewer_t::Instance().begin_interaction();
    interacting_ = true;
    app().memory_manager().begin_interaction();
}

void user_interface_impl::end_interaction()
{ 
    interacting_ = false;
    app().memory_manager().end_interaction();
	viewer_t::Instance().end_interaction();
	document_t::Instance().composition().end_interaction();
}

int user_interface_impl::start_frame() const
{
    return document_t::Instance().composition().start_frame();
}

int user_interface_impl::end_frame() const
{
    return document_t::Instance().composition().end_frame();
}

float user_interface_impl::frame() const
{
    return document_t::Instance().composition().frame();
}

void user_interface_impl::set_start_frame( int t)
{
    document_t::Instance().composition().set_start_frame( t);
    main_window()->time_slider()->update( document_t::Instance().composition().start_frame(),
										 document_t::Instance().composition().frame(),
										 document_t::Instance().composition().end_frame());

    render_composition_dialog_t::Instance().set_frame_range( document_t::Instance().composition().start_frame(),
														    document_t::Instance().composition().end_frame());

    render_flipbook_dialog_t::Instance().set_frame_range( document_t::Instance().composition().start_frame(),
									    				document_t::Instance().composition().end_frame());
}

void user_interface_impl::set_end_frame( int t)
{
    document_t::Instance().composition().set_end_frame( t);
    main_window()->time_slider()->update( document_t::Instance().composition().start_frame(), 
										 document_t::Instance().composition().frame(), 
										 document_t::Instance().composition().end_frame());

    render_composition_dialog_t::Instance().set_frame_range( document_t::Instance().composition().start_frame(),
														    document_t::Instance().composition().end_frame());

    render_flipbook_dialog_t::Instance().set_frame_range( document_t::Instance().composition().start_frame(),
										    				document_t::Instance().composition().end_frame());
}

void user_interface_impl::set_frame( int t)
{
    document_t::Instance().composition().set_frame( t);
    main_window()->time_slider()->update( document_t::Instance().composition().start_frame(),
										 document_t::Instance().composition().frame(),
										 document_t::Instance().composition().end_frame());

    inspector_t::Instance().update();
	update_anim_editors();
    viewer_t::Instance().frame_changed();
}

void user_interface_impl::update_anim_editors()
{
    anim_editor_t::Instance().update();
}

void user_interface_impl::fatal_error( const std::string& msg) const
{
    QMessageBox::critical( 0, "Fatal Error", msg.c_str());
}

void user_interface_impl::error( const std::string& msg) const
{
    QMessageBox::warning( ( QWidget *) main_window(), "Error", msg.c_str());
}

void user_interface_impl::inform( const std::string& msg) const
{
    QMessageBox::information( ( QWidget *) main_window(), "Info", msg.c_str());
}

bool user_interface_impl::question( const std::string& what, bool default_answer) const
{
    QMessageBox::StandardButton result;

    if( default_answer)
    {
        result = QMessageBox::question( ( QWidget *)  main_window(), "Question", what.c_str(),
                                        QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return result == QMessageBox::Yes;
    }
    else
    {
        result = QMessageBox::question( ( QWidget *) main_window(), "Question", what.c_str(),
                                        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        return result == QMessageBox::Yes;
    }
}

bool user_interface_impl::image_sequence_file_selector( boost::filesystem::path& p, bool& sequence, bool& relative) const
{
    std::string types( image_types_string().toStdString());
    return image_sequence_file_selector( "Open Image", types, p, sequence, relative);
}

bool user_interface_impl::image_sequence_file_selector( const std::string& title, const std::string& types, 
						       boost::filesystem::path& p, bool& sequence, bool& relative) const
{
    static bool was_relative = false;
    static bool was_sequence = true;

    QFileDialog dialog( 0, title.c_str(), QString::null, types.c_str());
	dialog.setOption( QFileDialog::DontUseNativeDialog, true);
    dialog.setFileMode( QFileDialog::ExistingFile);

    QWidget *extra_widgets = new QWidget( &dialog);
    QHBoxLayout *layout = new QHBoxLayout( extra_widgets);

    QCheckBox *relative_check = new QCheckBox( "Relative");

    if( document_t::Instance().has_file())
    {
        RAMEN_ASSERT( !document_t::Instance().composition().composition_dir().empty());
        relative_check->setChecked( was_relative);
    }
    else
    {
        was_relative = false;
        relative_check->setChecked( false);
        relative_check->setEnabled( false);
    }

    QCheckBox *sequence_check = new QCheckBox( "Sequence");
    sequence_check->setChecked( was_sequence);

    layout->addWidget( relative_check);
    layout->addWidget( sequence_check);

    QGridLayout *glayout = (QGridLayout *) dialog.layout();
    glayout->addWidget( extra_widgets, glayout->rowCount(), 0, 1, glayout->columnCount(), Qt::AlignLeft);

    dialog.show();

    QStringList filenames;

    if( dialog.exec())
    {
        filenames = dialog.selectedFiles();
        p = filenames[0].toStdString();

        relative = relative_check->isChecked();
        was_relative = relative;

        if( relative_check->isChecked())
        {
            boost::filesystem::path dir = document_t::Instance().file().parent_path();
            p = filesystem::make_relative_path( p, dir);
        }

        sequence = sequence_check->isChecked();
        was_sequence = sequence;
        return true;
    }

    return false;
}

// serialization
void read_ui_state( const serialization::yaml_iarchive_t& in)
{
	// TODO: implement this
}

void user_interface_impl::write_ui_state( serialization::yaml_oarchive_t& out) const
{
	// out << YAML::BeginMap;
	// save state( out)
	// out << YAML::EndMap;
}

// event filter
void user_interface_impl::start_long_process()
{ 
	RAMEN_ASSERT( !event_filter_installed_);
	
	cancelled_ = false;
	qApp->installEventFilter( this);
	event_filter_installed_ = true;
}

void user_interface_impl::process_events() { qApp->processEvents();}

bool user_interface_impl::process_cancelled() const	{ return cancelled_;}

void user_interface_impl::end_long_process()
{ 
	RAMEN_ASSERT( event_filter_installed_);	
	
	qApp->removeEventFilter( this);
	event_filter_installed_ = false;
}

bool user_interface_impl::eventFilter( QObject *watched, QEvent *event)
{
	switch( event->type())
	{
	case QEvent::KeyPress:
	{
		QKeyEvent *key_event = dynamic_cast<QKeyEvent*>( event);

		if( key_event->key() == Qt::Key_Escape)
			cancelled_ = true;

		return true;
	}
	break;
	
	case QEvent::KeyRelease:
	case QEvent::MouseButtonPress:
	case QEvent::MouseButtonRelease:
		return true;

	case QEvent::MouseMove:
	{
		QMouseEvent *mouse_event = dynamic_cast<QMouseEvent*>( event);
		return mouse_event->buttons() == Qt::NoButton;
	}
	break;
	
	default:
		return false; // pass all other events
	}
}

boost::unique_future<bool>& user_interface_impl::render_image( render::context_t context, render::image_node_renderer_t& renderer)
{
	RAMEN_ASSERT( !rendering_);
	
	cancelled_ = false;
	context.mode = render::interface_render;
	context.cancel = boost::bind( &user_interface_impl::process_cancelled, this);
	renderer.set_context( context);
	rendering_ = true;
    boost::unique_future<bool>& future( app().render_thread().render_image( renderer));

	if( future.is_ready())
	{
		rendering_ = false;
		return future;
	}

	/*
	start_long_process();

	while( !future.timed_wait( boost::posix_time::milliseconds( 30)))
		process_events();

	end_long_process();
	*/
	
	future.wait();
	rendering_ = false;
	return future;
}

QFont user_interface_impl::get_fixed_width_code_font()
{
	QFont font;
	font.setFamily( "Courier");
	font.setFixedPitch( true);
	font.setPointSize( 11);
	return font;
}

} // namespace
} // namespace
