# Copyright (c) 2011 Esteban Tovagliari
# Licensed under the terms of the CDDL License.
# See CDDL_LICENSE.txt for a copy of the license.

import logging

import PySide.QtCore as QtCore
import PySide.QtGui as QtGui

class main_window( QtGui.QMainWindow):
    def __init__( self, app, ui, parent = None):
        super( main_window, self).__init__( parent)
        logging.debug( 'Created main window')

        self.__app = app
        self.__ui = ui
        self.setWindowTitle( self.__app.full_version_name())

        self.__menu_bar = self.menuBar()

        self.__create_actions()
        self.__create_menus()

        self.setCorner( QtCore.Qt.TopRightCorner, QtCore.Qt.RightDockWidgetArea)
        self.setCorner( QtCore.Qt.BottomRightCorner, QtCore.Qt.RightDockWidgetArea)

        # inspector
        self.__inspector_dock = QtGui.QDockWidget( "Inspector", self)
        self.__inspector_dock.setObjectName( "inspector_dock")
        self.__inspector_dock.setAllowedAreas( QtCore.Qt.RightDockWidgetArea |
                                                QtCore.Qt.LeftDockWidgetArea |
                                                QtCore.Qt.BottomDockWidgetArea)
        self.__inspector_dock.setWidget( QtGui.QWidget())
        self.__add_dock_widget( QtCore.Qt.RightDockWidgetArea, self.__inspector_dock)

        # anim editor
        self.__anim_editor_dock = QtGui.QDockWidget( "Curve Editor", self)
        self.__anim_editor_dock.setObjectName( "anim_editor_dock")
        self.__anim_editor_dock.setAllowedAreas( QtCore.Qt.BottomDockWidgetArea |
                                                QtCore.Qt.TopDockWidgetArea |
                                                QtCore.Qt.LeftDockWidgetArea |
                                                QtCore.Qt.RightDockWidgetArea)
        self.__anim_editor_dock.setWidget( QtGui.QWidget())
        self.__add_dock_widget( QtCore.Qt.BottomDockWidgetArea, self.__anim_editor_dock)

        # Composition view
        self.__world_dock = QtGui.QDockWidget( "World", self)
        self.__world_dock.setObjectName( "world_dock");
        self.__world_dock.setAllowedAreas( QtCore.Qt.BottomDockWidgetArea |
                                          QtCore.Qt.TopDockWidgetArea |
                                          QtCore.Qt.LeftDockWidgetArea |
                                          QtCore.Qt.RightDockWidgetArea);

        self.__world_dock.setWidget( QtGui.QWidget());
        self.__add_dock_widget( QtCore.Qt.LeftDockWidgetArea, self.__world_dock)

        self.setCentralWidget( QtGui.QWidget() )

        # time toolbar
        self.addToolBar( QtCore.Qt.BottomToolBarArea, self.__create_time_toolbar())

        # create the status bar
        self.statusBar().showMessage( self.__app.full_version_name())

        screen_size = QtGui.qApp.desktop().availableGeometry();
        self.move( screen_size.left(), screen_size.top());
        self.resize( screen_size.width(), screen_size.height() - 40);

    def update_ui( self):
        window_title = self.__app.full_version_name() + ' - Untitled'

        if False: # document is dirty
            window_title += '*'

        self.setWindowTitle( window_title)
        self.__update_menus()

    def __create_time_toolbar( self):
        toolbar = QtGui.QToolBar( "Time Controls");
        toolbar.setObjectName( "time_controls");
        toolbar.setFloatable( False);
        toolbar.setMovable( False);
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

        self.__actions['new']     = self.__create_action( 'New'       , 'Ctrl+N', '__new_document()')
        self.__actions['open']    = self.__create_action( 'Open...'   , 'Ctrl+O', '__open_document()')
        self.__actions['save']    = self.__create_action( 'Save'      , 'Ctrl+S', '__save_document()')
        self.__actions['save_as'] = self.__create_action( 'Save As...', None    , '__save_document_as()')
        self.__actions['quit']    = self.__create_action( 'Quit'      , 'Ctrl+Q', '__quit()')

        self.__actions['undo']   = self.__create_action( 'Undo', 'Ctrl+Z')
        self.__actions['redo']   = self.__create_action( 'Redo', 'Ctrl+Shift+Z')
        self.__actions['delete'] = self.__create_action( 'Delete')
        self.__actions['prefs']  = self.__create_action( 'Preferences...')

        if self.__app.debug_build():
            self.__actions['run_tests'] = self.__create_action( 'Run tests', None, '__run_tests()')

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

        if self.__app.debug_build():
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
        self.__quit()

    # slots
    def __new_document( self):
        pass

    def __open_document( self):
        pass

    def __save_document( self):
        pass

    def __save_document_as( self):
        pass

    def __quit( self):
        self.__ui.quit()

    def __run_tests( self):
        self.__app.run_all_tests()
