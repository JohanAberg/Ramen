// Copyright (c) 2010 Esteban Tovagliari

#include<ramen/python/python.hpp>

#include<ramen/ui/main_window.hpp>

#include<ramen/version.hpp>

#include<iostream>
#include<map>

#include<boost/foreach.hpp>

#include<QApplication>
#include<QMenuBar>
#include<QMenu>
#include<QAction>
#include<QMessageBox>
#include<QVBoxLayout>
#include<QCloseEvent>
#include<QFileDialog>
#include<QDesktopServices>
#include<QDockWidget>
#include<QDesktopWidget>
#include<QUrl>
#include<QStatusBar>
#include<QSettings>
#include<QStringList>
#include<QFileInfo>

#include<ramen/version.hpp>

#include<ramen/ui/user_interface.hpp>
#include<ramen/ui/inspector/inspector.hpp>
#include<ramen/ui/add_node_command.hpp>
#include<ramen/ui/edit_commands.hpp>
#include<ramen/ui/time_controls.hpp>
#include<ramen/ui/compview/composition_view.hpp>
#include<ramen/ui/viewer/viewer.hpp>
#include<ramen/ui/anim/anim_editor.hpp>
#include<ramen/ui/render_composition.hpp>
#include<ramen/ui/widgets/time_slider.hpp>

#include<ramen/ui/dialogs/about_dialog.hpp>
#include<ramen/ui/dialogs/render_composition_dialog.hpp>
#include<ramen/ui/dialogs/render_flipbook_dialog.hpp>
#include<ramen/ui/dialogs/composition_settings_dialog.hpp>
#include<ramen/ui/dialogs/preferences_dialog.hpp>
#include<ramen/ui/dialogs/multiline_alert.hpp>

#include<ramen/app/application.hpp>
#include<ramen/app/document.hpp>
#include<ramen/app/export_selected_nodes.hpp>
#include<ramen/app/import_composition.hpp>

#include<ramen/memory/manager.hpp>

#include<ramen/nodes/graph_algorithm.hpp>
#include<ramen/nodes/node_factory.hpp>
#include<ramen/nodes/node_output_interface.hpp>
#include<ramen/nodes/image_node.hpp>
#include<ramen/nodes/image/roto/roto_node.hpp>
#include<ramen/nodes/image/color/cdl_node.hpp>

#include<ramen/imageio/import_multichannel_exr.hpp>

#include<ramen/system/system.hpp>

#include<ramen/undo/stack.hpp>

#include<ramen/render/render_sequence.hpp>

#include<ramen/flipbook/factory.hpp>
#include<ramen/flipbook/render_flipbook.hpp>

#include<ramen/ui/python/console.hpp>
#include<ramen/ui/python/editor.hpp>

namespace ramen
{
namespace ui
{

const int main_window_t::max_recently_opened_files = 5;

const char *main_window_t::document_extension()		{ return ".rmn";}
const char *main_window_t::file_dialog_extension()	{ return "Ramen Composition (*.rmn)";}

main_window_t::main_window_t() : QMainWindow()
{
	menubar_ = menuBar();

	recently_opened_.assign( max_recently_opened_files, (QAction *) 0);

    create_actions();
    create_menus();

   // docks
    setCorner( Qt::TopRightCorner, Qt::RightDockWidgetArea);
    setCorner( Qt::BottomRightCorner, Qt::RightDockWidgetArea);

    // Inspector
    inspector_dock_ = new QDockWidget( "Inspector", this);
    inspector_dock_->setObjectName( "inspector_dock");
    inspector_dock_->setAllowedAreas( Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea | Qt::BottomDockWidgetArea);
    inspector_dock_->setWidget( inspector_t::Instance().widget());
    add_dock_widget( Qt::RightDockWidgetArea, inspector_dock_);

    // vcr controls
    time_controls_dock_ = new QDockWidget( "Time Controls", this);
    time_controls_dock_->setObjectName( "time_controls_dock");
    time_controls_dock_->setAllowedAreas( Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea | Qt::BottomDockWidgetArea);
    time_controls_dock_->setWidget( time_controls_t::Instance().widget());
    add_dock_widget( Qt::RightDockWidgetArea, time_controls_dock_);

	// python
	py_console_dock_ = new QDockWidget( "Python Console", this);
	py_console_dock_->setObjectName( "py_console_dock");
	py_console_dock_->setAllowedAreas( Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea |
										Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

	py_console_dock_->setWidget( python::console_t::Instance().widget());
	add_dock_widget( Qt::BottomDockWidgetArea, py_console_dock_);

	py_editor_dock_ = new QDockWidget( "Python Editor", this);
	py_editor_dock_->setObjectName( "py_editor_dock");
	py_editor_dock_->setAllowedAreas( Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea |
										Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

	py_editor_dock_->setWidget( python::editor_t::Instance().widget());
	add_dock_widget( Qt::LeftDockWidgetArea, py_editor_dock_);

    // anim editor dock
    anim_editor_dock_ = new QDockWidget( "Curve Editor", this);
    anim_editor_dock_->setObjectName( "anim_editor_dock");
    anim_editor_dock_->setAllowedAreas( Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea |
										Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    anim_editor_dock_->setWidget( anim_editor_t::Instance().widget());
    add_dock_widget( Qt::BottomDockWidgetArea, anim_editor_dock_);
	tabifyDockWidget( py_console_dock_, anim_editor_dock_);

    // Composition view
	{
	    composition_dock_ = new QDockWidget( "Composition", this);
		composition_dock_->setObjectName( "composition_dock");
	    composition_dock_->setAllowedAreas( Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea |
											Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);


		QWidget *all_comp_view = new QWidget();
		QVBoxLayout *layout = new QVBoxLayout();
	    layout->setContentsMargins( 5, 5, 5, 5);

	    comp_view_ = new composition_view_t();
		layout->addWidget( comp_view_);

		QFrame *separator = new QFrame();
	    separator->setFrameStyle( QFrame::HLine | QFrame::Raised);
	    separator->setLineWidth( 1);
	    layout->addWidget( separator);

		layout->addWidget( comp_view_->create_toolbar());
		all_comp_view->setLayout( layout);

		composition_dock_->setWidget( all_comp_view);
	    add_dock_widget( Qt::LeftDockWidgetArea, composition_dock_);
		tabifyDockWidget( py_editor_dock_, composition_dock_);
	}
	
    // image view
    QWidget *viewer_widget = new QWidget();
    time_slider_ = new time_slider_t();
    connect( time_slider_, SIGNAL( start_frame_changed( int)), &user_interface_t::Instance(), SLOT( set_start_frame( int)));
    connect( time_slider_, SIGNAL( end_frame_changed( int)), &user_interface_t::Instance(), SLOT( set_end_frame( int)));
    connect( time_slider_, SIGNAL( time_changed( int)), &user_interface_t::Instance(), SLOT( set_frame( int)));

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setSpacing( 0);
    layout->setContentsMargins( 0, 0, 0, 0);
    layout->addWidget( viewer_t::Instance().widget());
    layout->addWidget( time_slider_);
    viewer_widget->setLayout( layout);
    setCentralWidget( viewer_widget);

    // add hotkeys actions here

    // create the status bar
    statusBar()->showMessage( RAMEN_NAME_FULL_VERSION_STR);

    QRect screen_size = qApp->desktop()->availableGeometry();
    move( screen_size.left(), screen_size.top());
    resize( screen_size.width(), screen_size.height() - 40);
    setWindowTitle( "Ramen");

    setWindowIcon( QIcon( ":small_app_icon.png"));
}

void main_window_t::add_dock_widget( Qt::DockWidgetArea area, QDockWidget *dock)
{
	addDockWidget( area, dock);
	view_->addAction( dock->toggleViewAction());
}

void main_window_t::closeEvent( QCloseEvent *event)
{
    quit();

    if( user_interface_t::Instance().quitting())
		event->accept();
    else
		event->ignore();
}

void main_window_t::create_actions()
{
    new_ = new QAction( "New", this);
    new_->setShortcut( QString( "Ctrl+N"));
    new_->setShortcutContext( Qt::ApplicationShortcut);
    connect( new_, SIGNAL( triggered()), this, SLOT( new_document()));

    open_ = new QAction( "Open...", this);
    open_->setShortcut( QString( "Ctrl+O"));
    open_->setShortcutContext( Qt::ApplicationShortcut);
    connect( open_, SIGNAL( triggered()), this, SLOT( open_document()));

    for( int i = 0; i < max_recently_opened_files; ++i)
    {
        recently_opened_[i] = new QAction( this);
        recently_opened_[i]->setVisible( false);
        connect( recently_opened_[i], SIGNAL( triggered()), this, SLOT( open_recent_document()));
    }

    save_ = new QAction( "Save", this);
    save_->setShortcut( QString( "Ctrl+S"));
    save_->setShortcutContext( Qt::ApplicationShortcut);
    connect( save_, SIGNAL( triggered()), this, SLOT( save_document()));

    save_as_ = new QAction( "Save As...", this);
    connect( save_as_, SIGNAL( triggered()), this, SLOT( save_document_as()));

    quit_ = new QAction( "Quit", this);
    quit_->setShortcut( QString( "Ctrl+Q"));
    quit_->setShortcutContext( Qt::ApplicationShortcut);
    connect( quit_, SIGNAL( triggered()), this, SLOT( quit()));

    import_comp_ = new QAction( "Import Composition...", this);
    connect( import_comp_, SIGNAL( triggered()), this, SLOT( import_composition()));

    export_sel_ = new QAction( "Export Selection...", this);
    export_sel_->setEnabled( false);
    connect( export_sel_, SIGNAL( triggered()), this, SLOT( export_selection()));

    import_multiexr_ = new QAction( "Import All EXR Layers...", this);
    connect( import_multiexr_, SIGNAL( triggered()), this, SLOT( import_multichannel_exr()));

	import_roto_ = new QAction( "Import Roto...", this);
	import_roto_->setEnabled( false);
    connect( import_roto_, SIGNAL( triggered()), this, SLOT( import_roto()));

	import_cdl_ = new QAction( "Import CDL...", this);
	connect( import_cdl_, SIGNAL( triggered()), this, SLOT( import_cdl()));
	
	export_roto_ = new QAction( "Export Roto...", this);
	export_roto_->setEnabled( false);
    connect( export_roto_, SIGNAL( triggered()), this, SLOT( export_roto()));

	export_cdl_ = new QAction( "Export CDL...", this);
	connect( export_cdl_, SIGNAL( triggered()), this, SLOT( export_cdl()));
	
    undo_ = new QAction( "Undo", this);
    undo_->setShortcut( QString( "Ctrl+Z"));
    undo_->setShortcutContext( Qt::ApplicationShortcut);
    connect( undo_, SIGNAL( triggered()), this, SLOT( undo()));

    redo_ = new QAction( "Redo", this);
    redo_->setShortcut( QString( "Ctrl+Shift+Z"));
    redo_->setShortcutContext( Qt::ApplicationShortcut);
    connect( redo_, SIGNAL( triggered()), this, SLOT( redo()));

    ignore_ = new QAction( "Ignore", this);
    ignore_->setShortcut( QString( "Ctrl+I"));
    ignore_->setShortcutContext( Qt::ApplicationShortcut);
    connect( ignore_, SIGNAL( triggered()), this, SLOT( ignore_nodes()));

    delete_ = new QAction( "Delete", this);
    connect( delete_, SIGNAL( triggered()), this, SLOT( delete_nodes()));

    duplicate_ = new QAction( "Duplicate", this);
    duplicate_->setShortcut( QString( "Ctrl+D"));
    duplicate_->setShortcutContext( Qt::ApplicationShortcut);
    connect( duplicate_, SIGNAL( triggered()), this, SLOT( duplicate_nodes()));

	extract_ = new QAction( "Extract", this);
    connect( extract_, SIGNAL( triggered()), this, SLOT( extract_nodes()));
	
    group_ = new QAction( "Group", this);
    ungroup_ = new QAction( "Ungroup", this);

    clear_cache_ = new QAction( "Clear Image Cache", this);
    connect( clear_cache_, SIGNAL( triggered()), this, SLOT( clear_cache()));

    preferences_ = new QAction( "Preferences...", this);
    connect( preferences_, SIGNAL( triggered()), this, SLOT( show_preferences_dialog()));

    comp_settings_ = new QAction( "Composition Settings...", this);
    comp_settings_->setShortcut( QString( "Ctrl+K"));
    comp_settings_->setShortcutContext( Qt::ApplicationShortcut);
    connect( comp_settings_, SIGNAL( triggered()), this, SLOT( show_composition_settings_dialog()));

    comp_flipbook_ = new QAction( "Render Flipbook...", this);
    connect( comp_flipbook_, SIGNAL( triggered()), this, SLOT( render_flipbook()));

    comp_render_ = new QAction( "Render Composition...", this);
    comp_render_->setShortcut( QString( "Ctrl+R"));
    comp_render_->setShortcutContext( Qt::ApplicationShortcut);
    connect( comp_render_, SIGNAL( triggered()), this, SLOT( render_composition()));

    about_ = new QAction( "About Ramen...", this);
    connect( about_, SIGNAL( triggered()), this, SLOT( show_about_box()));

    project_web_ = new QAction( tr( "Project Website..."), this);
    connect( project_web_, SIGNAL( triggered()), this, SLOT( go_to_project_website()));
	
    // non-menu actions
    next_frame_ = new QAction( this);
    next_frame_->setShortcut( Qt::Key_Right);
    next_frame_->setShortcutContext( Qt::WidgetWithChildrenShortcut);
    connect( next_frame_, SIGNAL( triggered()), &(time_controls_t::Instance()), SLOT( next_frame()));
    addAction( next_frame_);

    prev_frame_ = new QAction( this);
    prev_frame_->setShortcut( Qt::Key_Left);
    prev_frame_->setShortcutContext( Qt::WidgetWithChildrenShortcut);
    connect( prev_frame_, SIGNAL( triggered()), &(time_controls_t::Instance()), SLOT( prev_frame()));
    addAction( prev_frame_);
}

void main_window_t::create_menus()
{
    file_ = menubar_->addMenu( "File");
	file_->addAction( new_);
	file_->addAction( open_);

	open_recent_ = file_->addMenu( "Open Recent");
    for( int i = 0; i < max_recently_opened_files; ++i)
		open_recent_->addAction( recently_opened_[i]);

	init_recent_files_menu();

	file_->addAction( save_);
	file_->addAction( save_as_);
	file_->addSeparator();

	import_ = file_->addMenu( "Import");
	export_ = file_->addMenu( "Export");
	create_import_export_menus();

	file_->addSeparator();
	file_->addAction( quit_);

    edit_ = menubar_->addMenu( "Edit");
    edit_->addAction( undo_);
	edit_->addAction( redo_);
	edit_->addSeparator();
	edit_->addAction( ignore_);
	edit_->addAction( delete_);
	edit_->addAction( duplicate_);
	edit_->addAction( extract_);
	edit_->addSeparator();
    //edit_->addAction( group_);
    //edit_->addAction( ungroup_);
    //edit_->addSeparator();
	edit_->addAction( clear_cache_);
	edit_->addSeparator();
	edit_->addAction( preferences_);

    composition_ = menubar_->addMenu( "Composition");
	composition_->addAction( comp_settings_);
	composition_->addSeparator();
	composition_->addAction( comp_flipbook_);
	composition_->addAction( comp_render_);

    create_node_actions();

    for( int i = 0; i < node_menus_.size(); ++i)
	{
		node_menus_[i]->menu()->setTearOffEnabled( true);
        menubar_->addMenu( node_menus_[i]->menu());
	}

    view_ = menubar_->addMenu( "Window");

    help_ = menubar_->addMenu( "Help");
	help_->addAction( about_);
	help_->addAction( project_web_);	
}

void main_window_t::create_import_export_menus()
{
    import_->addAction( import_comp_);
    import_->addSeparator();
    import_->addAction( import_multiexr_);
	import_->addAction( import_cdl_);
	import_->addAction( import_roto_);
	// add plugin import menus here
	
    export_->addAction( export_sel_);
	export_->addSeparator();
	export_->addAction( export_cdl_);
	export_->addAction( export_roto_);
	// add plugin export menus here
}

node_menu_t *main_window_t::find_node_menu( const std::string& s)
{
    for( int i = 0; i < node_menus_.size(); ++i)
    {
        if( node_menus_[i]->name() == s)
            return node_menus_[i];
    }

    node_menu_t *m = new node_menu_t( s);
    node_menus_.push_back( m);
    return m;
}

void main_window_t::create_node_actions()
{
    // Add some default menus & submenus
    node_menu_t *m = new node_menu_t( "Image");
    node_menus_.push_back( m);
        m->add_submenu( "Input");
        m->add_submenu( "Channel");
        m->add_submenu( "Color");
        m->add_submenu( "Key");
        m->add_submenu( "Matte");
        m->add_submenu( "Filter");
        m->add_submenu( "Noise");
        m->add_submenu( "Distort");
        //m->add_submenu( "Inpaint");
        m->add_submenu( "Transform");
        m->add_submenu( "Track");
        m->add_submenu( "Layer");
        //m->add_submenu( "Render");
        m->add_submenu( "Lighting");
        m->add_submenu( "Tonemap");
        m->add_submenu( "Time");
        m->add_submenu( "Util");
		//m->add_submenu( "Video");
        m->add_submenu( "Output");

	// sort the list of registered nodes
	node_factory_t::Instance().sort_by_menu_item();

    // add our builtin nodes first
    BOOST_FOREACH( const node_metaclass_t& mclass, node_factory_t::Instance().registered_nodes())
    {
        if( mclass.ui_visible && node_factory_t::Instance().is_latest_version( mclass.id))
        {
            node_menu_t *menu = find_node_menu( mclass.menu);
            QAction *act = new QAction( QString( mclass.menu_item.c_str()), this);
            connect( act, SIGNAL( triggered()), this, SLOT( create_node()));
            create_node_actions_[act] = mclass.id;
            menu->add_action( mclass.submenu, act);
        }
    }
}

const std::vector<node_menu_t*>& main_window_t::node_menus() const
{
	RAMEN_ASSERT( !node_menus_.empty());
	return node_menus_;
}

bool main_window_t::can_close_document()
{
    if( document_t::Instance().dirty())
    {
        int r = QMessageBox::warning( this, "Ramen", "The composition has been modified.\n"
                                        "Do you want to save your changes?", QMessageBox::Yes | QMessageBox::Default,
                                        QMessageBox::No, QMessageBox::Cancel | QMessageBox::Escape);

        if (r == QMessageBox::Yes)
        {
            save_document();
            return document_t::Instance().dirty();
        }
        else
            if( r == QMessageBox::Cancel)
                return false;
    }

    return true;
}

void main_window_t::new_document()
{
    if( can_close_document())
        user_interface_t::Instance().create_new_document();
}

void main_window_t::open_document()
{
    if( can_close_document())
    {
        QString fname = QFileDialog::getOpenFileName( 0, "Open Composition", QString::null, file_dialog_extension(),
													  0, QFileDialog::DontUseNativeDialog);

        if( !( fname.isEmpty()))
        {
            boost::filesystem::path p( fname.toStdString());
            ui::user_interface_t::Instance().open_document( p);
        }
    }
}

void main_window_t::open_recent_document()
{
    if( can_close_document())
    {
        QAction *action = qobject_cast<QAction *>(sender());

        if( action)
        {
            QString fname = action->data().toString();
            boost::filesystem::path p( fname.toStdString());
            ui::user_interface_t::Instance().open_document( p);
        }
    }
}

void main_window_t::save_document()
{
    if( document_t::Instance().has_file())
	{
        ui::user_interface_t::Instance().save_document();
        ui::user_interface_t::Instance().update();
	}
    else
        save_document_as();
}

void main_window_t::save_document_as()
{
    QString fname = QFileDialog::getSaveFileName( 0, "Save Composition As", QString::null, file_dialog_extension(),
												  0, QFileDialog::DontUseNativeDialog);

    if( !(fname.isEmpty()))
    {
        boost::filesystem::path p( fname.toStdString());

        if( p.extension() == std::string())
            p.replace_extension( document_extension());

		boost::filesystem::path old_file = document_t::Instance().file();
        document_t::Instance().set_file( p);

        if( !ui::user_interface_t::Instance().save_document())
		{
			// save was not successful, restore the relative paths
			// to the state before trying to save.
			if( !old_file.empty())
				document_t::Instance().set_file( old_file);
		}
		else
		{
			update_recent_files_menu( document_t::Instance().file());
			undo::stack_t::Instance().clear();
		}
		
		user_interface_t::Instance().update();		
    }
}

void main_window_t::import_composition()
{
    QString fname = QFileDialog::getOpenFileName( 0, "Open Composition", QString::null, file_dialog_extension(),
												  0, QFileDialog::DontUseNativeDialog);

    if( !( fname.isEmpty()))
    {
        try
        {
            boost::filesystem::path p( fname.toStdString());
            std::auto_ptr<serialization::yaml_iarchive_t> in( ramen::import_composition( p));
            document_t::Instance().set_dirty( true);
            undo::stack_t::Instance().clear();
			
			// report errors to the user
			std::string err = in->errors();
			
			if( !err.empty())
				multiline_alert_t::Instance().show_alert( "Errors during file open", err);
        }
        catch( std::exception& e)
        {
            QMessageBox::warning( this, "Error opening document", "Old version or corrupted composition");
        }

		ui::user_interface_t::Instance().update();
    }
}

void main_window_t::import_multichannel_exr()
{
    boost::filesystem::path p;
    bool relative, sequence;

    if( ui::user_interface_t::Instance().image_sequence_file_selector( "Import EXR", "OpenEXR image (*.exr)", p, sequence, relative))
		imageio::import_multichannel_exr( p, relative, sequence);
}

void main_window_t::export_selection()
{
    QString fname = QFileDialog::getSaveFileName( 0, "Save Composition As", QString::null, file_dialog_extension(),
												  0, QFileDialog::DontUseNativeDialog);

    if( !(fname.isEmpty()))
    {
        boost::filesystem::path p( fname.toStdString());

        if( p.extension() == std::string())
            p.replace_extension( document_extension());

        export_selected_nodes( p);
    }
}

void main_window_t::import_roto()
{
    QString fname = QFileDialog::getOpenFileName( 0, "Import Roto", QString::null,
														"Ramen Shapes (*.yaml);;"  
														"Autodesk GMask (*.gmask);;"
														  "Shake SSF (*.ssf);;"
														  "Nuke (*.nk)", 0, QFileDialog::DontUseNativeDialog);
    if( !( fname.isEmpty()))
		ui::user_interface_t::Instance().error( "Feature not implemented yet.");
}

void main_window_t::import_cdl()
{
    QString fname = QFileDialog::getOpenFileName( 0, "Import CDL Transform", QString::null, "CDL transform (*.cc)",
												  0, QFileDialog::DontUseNativeDialog);

    if( !( fname.isEmpty()))
	{
		boost::intrusive_ptr<image::cdl_node_t> p( new image::cdl_node_t());
		
		if( !p.get())
		{
			user_interface_t::Instance().error( std::string( "Couldn't create cdl node"));
			return;
		}
		
		try
		{
			p->set_composition( &( document_t::Instance().composition()));
			p->create_params();
			p->create_manipulators();
			p->read_from_file( boost::filesystem::path( fname.toStdString()));
		}
		catch( std::exception& e)
		{
			user_interface_t::Instance().error( std::string( "Couldn't create cdl node ") + e.what());
			return;
		}
	
		composition_view()->place_node( p.get());
		node_t *n = p.get(); // save for later use
		std::auto_ptr<undo::command_t> c( new undo::add_node_command_t( p, 0));
		document_t::Instance().composition().deselect_all();
		n->select( true);
		c->redo();
		undo::stack_t::Instance().push_back( c);
		ui::user_interface_t::Instance().update();
	}
}

void main_window_t::export_roto()
{
    QString fname = QFileDialog::getSaveFileName( 0, "Export Roto...", QString::null,
													"Roto Files (*.yaml *.gmask *.ssf *.nk)",
												  0, QFileDialog::DontUseNativeDialog);

    if( !(fname.isEmpty()))
		ui::user_interface_t::Instance().error( "Feature not implemented yet.");
}

void main_window_t::export_cdl()
{
    QString fname = QFileDialog::getSaveFileName( 0, "Export CDL Transform...", QString::null, "CDL Transform (*.cc)",
												  0, QFileDialog::DontUseNativeDialog);

    if( !(fname.isEmpty()))
	{
		node_t *n = document_t::Instance().composition().selected_node();
		image::cdl_node_t *cdl_node = dynamic_cast<image::cdl_node_t*>( n);
		RAMEN_ASSERT( cdl_node);

		cdl_node->write_to_file( boost::filesystem::path( fname.toStdString()));
	}
}

void main_window_t::quit()
{
    if( document_t::Instance().dirty())
    {
        int r = QMessageBox::warning( this, "Ramen",
                        "The composition has been modified.\n"
                            "Do you want to save your changes before quitting?",
                        QMessageBox::Yes | QMessageBox::Default,
                        QMessageBox::No,
                        QMessageBox::Cancel | QMessageBox::Escape);

        switch( r)
        {
            case QMessageBox::Yes:
            {
                save_document();

                    // if the document is still dirty, it means
                    // save was cancelled, so we return without quitting
                if( document_t::Instance().dirty())
					return;
            }
            break;

            case QMessageBox::No:
                user_interface_t::Instance().quit();
            break;

            case QMessageBox::Cancel:
            return;
        }
    }

    user_interface_t::Instance().quit();
}

void main_window_t::undo()
{
    undo::stack_t::Instance().undo();
    user_interface_t::Instance().update();
}

void main_window_t::redo()
{
    undo::stack_t::Instance().redo();
    user_interface_t::Instance().update();
}

void main_window_t::ignore_nodes()
{
    std::auto_ptr<undo::ignore_nodes_command_t> c( new undo::ignore_nodes_command_t());

    BOOST_FOREACH( node_ptr_t n, document_t::Instance().composition().nodes())
    {
        if( n->selected())
            c->add_node( n.get());
    }

    c->redo();
    undo::stack_t::Instance().push_back( c);
    ui::user_interface_t::Instance().update();
}

void main_window_t::delete_nodes()
{
	bool autoconnect = true;

    if( !document_t::Instance().composition().any_selected())
        return;

    std::auto_ptr<undo::delete_command_t> c( new undo::delete_command_t());

    BOOST_FOREACH( edge_t& e, document_t::Instance().composition().edges())
    {
        if( e.src->selected() && !(e.dst->selected()))
            c->add_dependent_node( e.dst);
    }

    BOOST_FOREACH( node_ptr_t n, document_t::Instance().composition().nodes())
    {
        if( n->selected())
            c->add_node( n.get());
    }

    BOOST_FOREACH( edge_t& e, document_t::Instance().composition().edges())
    {
        if( e.src->selected() || e.dst->selected())
            c->add_edge_to_remove( e);
    }

	if( autoconnect)
	{
		std::vector<edge_t> edges_to_add;
		
		BOOST_FOREACH( node_ptr_t n, document_t::Instance().composition().nodes())
		{
			if( n->selected())
			{
				if( n->num_outputs() == 0)
					continue;
				
				node_t *src = 0;
				
				// find first input
				for( int i = 0; i < n->num_inputs(); ++i)
				{
					if( n->input( i) && !n->input( i)->selected())
					{
						src = n->input( i);
						break;
					}
				}

				if( src)
				{
					breadth_first_out_edges_apply( *n, boost::bind( &undo::delete_command_t::add_candidate_edge,
																			  _1, src, boost::ref( edges_to_add)));
				}
			}
		}
		
		std::stable_sort( edges_to_add.begin(), edges_to_add.end(), &undo::delete_command_t::edge_less);
		std::unique( edges_to_add.begin(), edges_to_add.end(), &undo::delete_command_t::edge_compare);
		
		for( int i = 0; i < edges_to_add.size(); ++i)
			c->add_edge_to_add( edges_to_add[i]);
	}
	
    c->redo();
    undo::stack_t::Instance().push_back( c);
    ui::user_interface_t::Instance().update();
}

void main_window_t::duplicate_nodes()
{
    std::map<node_t*, node_t*> relation;
    std::auto_ptr<undo::duplicate_command_t> c( new undo::duplicate_command_t());

    BOOST_FOREACH( node_ptr_t& n, document_t::Instance().composition().nodes())
    {
        if( n->selected())
        {
            node_ptr_t nclone = n->clone();
            nclone->offset_location( Imath::V2f( 20, 20));
            c->add_node( nclone);
            relation[ n.get()] = nclone.get();
        }
    }

    BOOST_FOREACH( edge_t& e, document_t::Instance().composition().edges())
    {
        if( e.src->selected() && e.dst->selected())
            c->add_edge( edge_t( relation[e.src], relation[e.dst], e.port));
    }

    document_t::Instance().composition().deselect_all();
    c->redo();
    undo::stack_t::Instance().push_back( c);
    ui::user_interface_t::Instance().update();
}

void main_window_t::extract_nodes()
{
	bool autoconnect = true;

    if( !document_t::Instance().composition().any_selected())
        return;

    std::auto_ptr<undo::extract_command_t> c( new undo::extract_command_t());

    BOOST_FOREACH( edge_t& e, document_t::Instance().composition().edges())
    {
        if( e.src->selected() && !(e.dst->selected()))
            c->add_dependent_node( e.dst);
    }

    BOOST_FOREACH( edge_t& e, document_t::Instance().composition().edges())
    {
        if( e.src->selected() || e.dst->selected())
            c->add_edge_to_remove( e);
    }

	if( autoconnect)
	{
		std::vector<edge_t> edges_to_add;
		
		BOOST_FOREACH( node_ptr_t n, document_t::Instance().composition().nodes())
		{
			if( n->selected())
			{
				if( n->num_outputs() == 0)
					continue;
				
				node_t *src = 0;
				
				// find first input
				for( int i = 0; i < n->num_inputs(); ++i)
				{
					if( n->input( i) && !n->input( i)->selected())
					{
						src = n->input( i);
						break;
					}
				}

				if( src)
					breadth_first_out_edges_apply( *n, boost::bind( &undo::delete_command_t::add_candidate_edge, _1, src, boost::ref( edges_to_add)));
			}
		}
		
		std::stable_sort( edges_to_add.begin(), edges_to_add.end(), &undo::delete_command_t::edge_less);
		std::unique( edges_to_add.begin(), edges_to_add.end(), &undo::delete_command_t::edge_compare);
		
		for( int i = 0; i < edges_to_add.size(); ++i)
			c->add_edge_to_add( edges_to_add[i]);
	}
	
    c->redo();
    undo::stack_t::Instance().push_back( c);
    ui::user_interface_t::Instance().update();
}

void main_window_t::clear_cache()
{
    app().memory_manager().clear_caches();
}

void main_window_t::show_composition_settings_dialog()
{
    composition_settings_dialog_t::Instance().exec_dialog();
}

void main_window_t::render_composition()
{
    bool any_output = ( render::count_output_nodes( document_t::Instance().composition()) != 0);

    if( !any_output)
    {
        QMessageBox::warning( this, "Ramen", "No output nodes in composition");
        return;
    }
	
	bool any_output_selected = ( render::count_output_nodes( document_t::Instance().composition(), true) != 0);

    render_composition_dialog_t::Instance().set_any_output_selected( any_output_selected);

    int result = render_composition_dialog_t::Instance().exec();

    if( result == QDialog::Accepted)
    {
		int start = render_composition_dialog_t::Instance().start_frame();
		int end = render_composition_dialog_t::Instance().end_frame();
		
		if( end < start)
			return;
		
		ui::render_composition( document_t::Instance().composition(),
								start, end, render_composition_dialog_t::Instance().proxy_level(),
								render_composition_dialog_t::Instance().resolution(),
								render_composition_dialog_t::Instance().mblur_extra_samples(),
								render_composition_dialog_t::Instance().mblur_shutter_factor(),
								render_composition_dialog_t::Instance().selected_only());
    }
}

void main_window_t::render_flipbook()
{
    int result = render_flipbook_dialog_t::Instance().exec();

    if( result == QDialog::Accepted)
    {
		if( image_node_t *n = dynamic_cast<image_node_t*>( document_t::Instance().composition().selected_node()))
		{
			int frame_rate = document_t::Instance().composition().frame_rate();
			std::string display_device = render_flipbook_dialog_t::Instance().display_device();
			std::string display_transform = render_flipbook_dialog_t::Instance().display_transform();
			int start	= render_flipbook_dialog_t::Instance().start_frame();
			int end	= render_flipbook_dialog_t::Instance().end_frame();
			int subsample =  render_flipbook_dialog_t::Instance().resolution();
			int proxy_level = render_flipbook_dialog_t::Instance().proxy_level();
			int mb_extra_samples = render_flipbook_dialog_t::Instance().mblur_extra_samples();
			float mb_shutter_factor = render_flipbook_dialog_t::Instance().mblur_shutter_factor();
	
			flipbook::flipbook_t *flip = flipbook::factory_t::Instance().create( render_flipbook_dialog_t::Instance().flipbook(),
																				   frame_rate, display_device, display_transform);

			if( flip)
			{
				if( flipbook::render_flipbook( flip, n, start, end, proxy_level, subsample, mb_extra_samples, mb_shutter_factor))
					flip->play();
			}
		}
	}
}

void main_window_t::show_preferences_dialog()
{
    preferences_dialog_t::Instance().exec_dialog();
}

void main_window_t::create_node()
{
    QAction *action = dynamic_cast<QAction*>( sender());

    std::string id( create_node_actions_[action]);
    node_ptr_t p = node_factory_t::Instance().create_by_id( id, true);
	
	if( !p.get())
	{
		//user_interface_t::Instance().error( std::string( "Couldn't create node ") + id);
		return;
	}
	
	try
	{
		p->set_composition( &( document_t::Instance().composition()));
		p->create_params();
		p->create_manipulators();
	}
	catch( std::exception& e)
	{
		user_interface_t::Instance().error( std::string( "Couldn't create node ") + id
											+ std::string( " ") + e.what());
		return;
	}

	// test to see if we can autoconnect
	node_t *src = document_t::Instance().composition().selected_node();
	
	if( src && src->has_output_plug() && p->num_inputs() != 0)
	{
		if( !document_t::Instance().composition().can_connect( src, p.get(), 0))
			src = 0;
	}
	else
		src = 0;
	
	if( src)
		composition_view()->place_node_near_node( p.get(), src);
	else
		composition_view()->place_node( p.get());
	
	node_t *n = p.get(); // save for later use
	std::auto_ptr<undo::command_t> c( new undo::add_node_command_t( p, src));
	document_t::Instance().composition().deselect_all();
	n->select( true);
	c->redo();
	undo::stack_t::Instance().push_back( c);
	ui::user_interface_t::Instance().update();
}

void main_window_t::show_about_box()
{
	about_dialog_t::Instance().exec();
}

void main_window_t::go_to_project_website()
{
    QDesktopServices::openUrl( QUrl( "http://ramencomp.blogspot.com/"));
}

void main_window_t::update()
{
    if( document_t::Instance().dirty())
        setWindowTitle( "Ramen *");
    else
        setWindowTitle( "Ramen");

    update_menus();
    time_slider_->update( document_t::Instance().composition().start_frame(),
			 document_t::Instance().composition().frame(),
			 document_t::Instance().composition().end_frame());

    composition_view()->update();
    time_controls_t::Instance().update();
}

void main_window_t::update_recent_files_menu( const boost::filesystem::path& p)
{
    QString fileName( filesystem::file_cstring( p));

    QSettings settings( "com.ramen.qt", "Ramen Recent Files");
    QStringList files = settings.value( "recent_file_list").toStringList();
    files.removeAll( fileName);
    files.prepend( fileName);

    while( files.size() > max_recently_opened_files)
        files.removeLast();

    settings.setValue("recent_file_list", files);

    int num_recent_files = std::min( files.size(), (int) max_recently_opened_files);

    for( int i = 0; i < num_recent_files; ++i)
    {
        QString stripped = QFileInfo( files[i]).fileName();
        QString text = tr("&%1 %2").arg(i + 1).arg( stripped);
        recently_opened_[i]->setText( text);
        recently_opened_[i]->setData( files[i]);
        recently_opened_[i]->setVisible( true);
    }

    for( int j = num_recent_files; j < max_recently_opened_files; ++j)
        recently_opened_[j]->setVisible( false);
}

void main_window_t::init_recent_files_menu()
{
    QSettings settings( "com.ramen.qt", "Ramen Recent Files");
    QStringList files = settings.value( "recent_file_list").toStringList();

    int num_recent_files = std::min( files.size(), (int) max_recently_opened_files);

    for( int i = 0; i < num_recent_files; ++i)
    {
        QString stripped = QFileInfo( files[i]).fileName();
        QString text = tr("&%1 %2").arg(i + 1).arg( stripped);
        recently_opened_[i]->setText( text);
        recently_opened_[i]->setData( files[i]);
        recently_opened_[i]->setVisible( true);
    }

    for( int j = num_recent_files; j < max_recently_opened_files; ++j)
        recently_opened_[j]->setVisible( false);
}

void main_window_t::update_menus()
{
    bool any_selected = document_t::Instance().composition().any_selected();
    node_t *n = document_t::Instance().composition().selected_node();

    save_->setEnabled( document_t::Instance().dirty());
    export_sel_->setEnabled( any_selected);

	if( undo::stack_t::Instance().undo_empty())
	{
		undo_->setText( "Undo");
	    undo_->setEnabled( false);
	}
	else
	{
		undo_->setText( QString( "Undo ") + undo::stack_t::Instance().last_undo_command().name().c_str());
	    undo_->setEnabled( true);
	}

	if( undo::stack_t::Instance().redo_empty())
	{
		redo_->setText( "Redo");
	    redo_->setEnabled( false);
	}
	else
	{
		redo_->setText( QString( "Redo ") + undo::stack_t::Instance().last_redo_command().name().c_str());
	    redo_->setEnabled( true);
	}
	
    ignore_->setEnabled( any_selected);
    delete_->setEnabled( any_selected);
    duplicate_->setEnabled( any_selected);
    extract_->setEnabled( any_selected);

    group_->setEnabled( false);
    ungroup_->setEnabled( false);

    if( n && dynamic_cast<image_node_t*>( n))
	{
        comp_flipbook_->setEnabled( true);
		
		/*
		if( dynamic_cast<image::roto_node_t*>( n))
			export_roto_->setEnabled( true);
		else
			export_roto_->setEnabled( false);
		*/
		
		if( dynamic_cast<image::cdl_node_t*>( n))
			export_cdl_->setEnabled( true);
		else
			export_cdl_->setEnabled( false);
	}
    else
	{
        comp_flipbook_->setEnabled( false);
		export_cdl_->setEnabled( false);
		//export_roto_->setEnabled( false);
	}
}

} // namespace
} // namespace