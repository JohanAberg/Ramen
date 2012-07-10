# Copyright (c) 2011 Esteban Tovagliari
# Licensed under the terms of the CDDL License.
# See CDDL_LICENSE.txt for a copy of the license.

import logging

import PySide.QtCore
import PySide.QtGui

from ramen.ui.main_window import main_window

class user_interface( object):
    def __init__( self):
        logging.debug( 'Created user interface')

    def run( self, args):
        app = PySide.QtGui.QApplication( args)

        self._main_window = main_window()
        self._main_window.show()
        return app.exec_()
