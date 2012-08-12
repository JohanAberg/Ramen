# Copyright (c) 2011 Esteban Tovagliari
# Licensed under the terms of the CDDL License.
# See CDDL_LICENSE.txt for a copy of the license.

import logging
import weakref

import PySide.QtCore as QtCore
import PySide.QtGui as QtGui

from ramen.ui.anim_editor_widget import anim_editor_widget
from ramen.ui.world_view_widget import world_view_widget
from ramen.ui.viewer_container_widget import viewer_container_widget
from ramen.ui.viewer_widget import viewer_widget
from ramen.ui.inspector_widget import inspector_widget
from ramen.ui.time_slider_widget import time_slider_widget
from ramen.ui.time_controls_widget import time_controls_widget
from ramen.ui.python_console_widget import python_console_widget
from ramen.ui.script_editor_widget import script_editor_widget

class main_window( QtGui.QMainWindow):
    def __init__( self, app, ui, parent = None):
        super( main_window, self).__init__( parent)
        logging.debug( 'Created main window')

        self.__app = weakref.ref( app)
        self.__ui = weakref.ref( ui)
        self.setWindowTitle( self.__app().full_version_name())

        self.__menu_bar = self.menuBar()
        self.__create_actions()
        self.__create_menus()

        self.setCorner( QtCore.Qt.TopRightCorner, QtCore.Qt.RightDockWidgetArea)
        self.setCorner( QtCore.Qt.BottomRightCorner, QtCore.Qt.RightDockWidgetArea)

        # world view
        self.__world_dock = QtGui.QDockWidget( "World", self)
        self.__world_dock.setObjectName( "world_dock");
        self.__world_dock.setAllowedAreas( QtCore.Qt.BottomDockWidgetArea |
                                          QtCore.Qt.TopDockWidgetArea |
                                          QtCore.Qt.LeftDockWidgetArea |
                                          QtCore.Qt.RightDockWidgetArea);

        self.__world_dock.setWidget( world_view_widget());
        self.__add_dock_widget( QtCore.Qt.LeftDockWidgetArea, self.__world_dock)

        # inspector
        self.__inspector_dock = QtGui.QDockWidget( "Inspector", self)
        self.__inspector_dock.setObjectName( "inspector_dock")
        self.__inspector_dock.setAllowedAreas( QtCore.Qt.RightDockWidgetArea |
                                                QtCore.Qt.LeftDockWidgetArea |
                                                QtCore.Qt.BottomDockWidgetArea)
        self.__inspector_dock.setWidget( inspector_widget())
        self.__add_dock_widget( QtCore.Qt.RightDockWidgetArea, self.__inspector_dock)

        # anim editor
        self.__anim_editor_dock = QtGui.QDockWidget( "Curve Editor", self)
        self.__anim_editor_dock.setObjectName( "anim_editor_dock")
        self.__anim_editor_dock.setAllowedAreas( QtCore.Qt.BottomDockWidgetArea |
                                                QtCore.Qt.TopDockWidgetArea |
                                                QtCore.Qt.LeftDockWidgetArea |
                                                QtCore.Qt.RightDockWidgetArea)
        self.__anim_editor_dock.setWidget( anim_editor_widget())
        self.__add_dock_widget( QtCore.Qt.BottomDockWidgetArea, self.__anim_editor_dock)

        # main viewer
        self.__viewer_container = viewer_widget()
        self.setCentralWidget( self.__viewer_container)

        # script editor
        self.__script_editor_dock = QtGui.QDockWidget( "Script Editor", self)
        self.__script_editor_dock.setObjectName( "script_editor_dock")
        self.__script_editor_widget = script_editor_widget()
        self.__script_editor_dock.setWidget( self.__script_editor_widget)
        self.__add_dock_widget( QtCore.Qt.LeftDockWidgetArea, self.__script_editor_dock)

        # python console
        self.__py_console_dock = QtGui.QDockWidget( "Python Console", self)
        self.__py_console_dock.setObjectName( "py_console_dock")
        self.__py_console_widget = python_console_widget()
        self.__py_console_dock.setWidget( self.__py_console_widget)
        self.__add_dock_widget( QtCore.Qt.BottomDockWidgetArea, self.__py_console_dock)

        # time toolbar
        self.__toolbar = self.__create_time_toolbar()
        self.addToolBar( QtCore.Qt.BottomToolBarArea, self.__toolbar)

        # create the status bar
        self.statusBar().showMessage( self.__app().full_version_name())

        screen_size = QtGui.qApp.desktop().availableGeometry();
        self.move( screen_size.left(), screen_size.top());
        self.resize( screen_size.width(), screen_size.height() - 40);

    def update_ui( self):
        window_title = self.__app().full_version_name()

        if self.__app().document().has_file():
            window_title += ' - ' + self.__app().document().file()
        else:
            window_title += ' - Untitled'

        if self.__app().document().dirty():
            window_title += '*'

        self.setWindowTitle( window_title)
        self.__update_menus()

    def __create_time_toolbar( self):
        toolbar = QtGui.QToolBar( "Time Controls");
        toolbar.setObjectName( "time_controls");
        toolbar.setFloatable( False);
        toolbar.setMovable( False);
        toolbar.setVisible( True)

        time_slider = time_slider_widget()
        time_slider.setSizePolicy( QtGui.QSizePolicy.Expanding, time_slider.sizePolicy().verticalPolicy())
        #connect( time_slider_, SIGNAL( start_frame_changed( int)), app().ui(), SLOT( set_start_frame( int)))
        #connect( time_slider_, SIGNAL( end_frame_changed( int)), app().ui(), SLOT( set_end_frame( int)))
        #connect( time_slider_, SIGNAL( time_changed( int)), app().ui(), SLOT( set_frame( int)))

        toolbar.addWidget( time_slider)
        toolbar.addSeparator()
        toolbar.addWidget( time_controls_widget( self.__ui().resources_path()))

        toolbar.addSeparator()
        flipbook_ = QtGui.QPushButton()
        flipbook_.setText( "Flipbook")
        flipbook_.setEnabled( False)
        #connect( flipbook_, SIGNAL( pressed()), this, SLOT( make_flipbook()))
        toolbar.addWidget( flipbook_)

        autokey_ = QtGui.QToolButton()
        autokey_.setText( "Autokey")
        autokey_.setCheckable( True)
        #autokey_.setChecked( app().document().composition().autokey())
        #connect( autokey_, SIGNAL( toggled( bool)), this, SLOT( set_autokey(bool)))
        toolbar.addWidget( autokey_)

        return toolbar

    def __create_action( self, name, shortcut = None, slot = None):
        action = QtGui.QAction( name, self)

        if shortcut:
            action.setShortcut( shortcut)
            action.setShortcutContext( QtCore.Qt.ApplicationShortcut)

        if slot:
            QtCore.QObject.connect( action, QtCore.SIGNAL( 'triggered()'), self, QtCore.SLOT( slot))

        return action

    def __create_actions( self):
        self.__actions = {}

        self.__actions['new']     = self.__create_action( 'New'       , 'Ctrl+N', 'new_document()')
        self.__actions['open']    = self.__create_action( 'Open...'   , 'Ctrl+O', 'open_document()')
        self.__actions['save']    = self.__create_action( 'Save'      , 'Ctrl+S', 'save_document()')
        self.__actions['save_as'] = self.__create_action( 'Save As...', None    , 'save_document_as()')
        self.__actions['quit']    = self.__create_action( 'Quit'      , 'Ctrl+Q', 'quit_app()')

        self.__actions['undo']   = self.__create_action( 'Undo', 'Ctrl+Z')
        self.__actions['redo']   = self.__create_action( 'Redo', 'Ctrl+Shift+Z')
        self.__actions['delete'] = self.__create_action( 'Delete')
        self.__actions['prefs']  = self.__create_action( 'Preferences...')

        if self.__app().debug_build():
            self.__actions['run_tests'] = self.__create_action( 'Run tests', None, 'run_tests()')

    def __create_menus( self):
        self.__file_menu = self.__menu_bar.addMenu( 'File')
        self.__file_menu.addAction( self.__actions['new'])
        self.__file_menu.addAction( self.__actions['open'])
        self.__file_menu.addAction( self.__actions['save'])
        self.__file_menu.addAction( self.__actions['save_as'])
        self.__file_menu.addSeparator()
        self.__file_menu.addAction( self.__actions['quit'])

        self.__edit_menu = self.__menu_bar.addMenu( 'Edit')
        self.__edit_menu.addAction( self.__actions['undo'])
        self.__edit_menu.addAction( self.__actions['redo'])
        self.__edit_menu.addSeparator()
        self.__edit_menu.addAction( self.__actions['delete'])
        self.__edit_menu.addSeparator()
        self.__edit_menu.addAction( self.__actions['prefs'])

        self.__window_menu = self.__menu_bar.addMenu( 'Window')

        self.__help_menu = self.__menu_bar.addMenu( 'Help')

        if self.__app().debug_build():
            self.__debug_menu = self.__menu_bar.addMenu( 'Debug')
            self.__debug_menu.addAction( self.__actions['run_tests'])

    def __add_dock_widget( self, area, dock):
        self.addDockWidget( area, dock)
        self.__window_menu.addAction( dock.toggleViewAction())

    def __update_menus( self):
        self.__actions['new'].setEnabled( False)
        self.__actions['open'].setEnabled( False)
        self.__actions['save'].setEnabled( False)
        self.__actions['save_as'].setEnabled( False)

        self.__actions['undo'].setEnabled( False)
        self.__actions['redo'].setEnabled( False)
        self.__actions['delete'].setEnabled( False)
        self.__actions['prefs'].setEnabled( False)

    # events
    def closeEvent( self, event):
        event.ignore()
        self.quit_app()

    # slots
    def new_document( self):
        pass

    def open_document( self):
        pass

    def save_document( self):
        pass

    def save_document_as( self):
        pass

    def quit_app( self):
        self.__ui().quit()

    def run_tests( self):
        self.__app().run_all_tests()
