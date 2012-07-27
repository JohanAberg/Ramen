# Copyright (c) 2011 Esteban Tovagliari
# Licensed under the terms of the CDDL License.
# See CDDL_LICENSE.txt for a copy of the license.

import logging

import PySide.QtCore as QtCore
import PySide.QtGui as QtGui

from ramen.ui.main_window import main_window

class user_interface( object):
    def __init__( self, app):
        self._app = app
        logging.debug( 'Created user interface')

    def run( self, args):
        app = QtGui.QApplication( args)

        self._main_window = main_window( self)
        self._main_window.show()
        return app.exec_()

    def quit( self):
        logging.debug( 'Application exiting')
        self._app.quit()
        QtGui.qApp.quit()
        logging.debug( 'Application finished')
