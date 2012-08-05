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

        self._app = app
        self._ui = ui
        self.setWindowTitle( self._app.full_version_name())

        self._menuBar = self.menuBar()

        self.createActions()
        self.createMenus()

    def createActions( self):
        self._actions = {}

        action = QtGui.QAction( 'Quit', self)
        action.setShortcut( 'Ctrl+Q')
        QtCore.QObject.connect( action, QtCore.SIGNAL( 'triggered()'), self, QtCore.SLOT( 'quitApp()'))
        self._actions['quit'] = action

        if self._app.debug_build():
            action = QtGui.QAction( 'Run tests', self)
            QtCore.QObject.connect( action, QtCore.SIGNAL( 'triggered()'), self, QtCore.SLOT( 'runTests()'))
            self._actions['runTests'] = action


    def createMenus( self):
        self._fileMenu = self._menuBar.addMenu( 'File')
        self._fileMenu.addAction( self._actions['quit'])

        if self._app.debug_build():
            self._debugMenu = self._menuBar.addMenu( 'Debug')
            self._debugMenu.addAction( self._actions['runTests'])

    # slots
    def quitApp( self):
        self._ui.quit()

    def runTests( self):
        self._app.run_all_tests()
