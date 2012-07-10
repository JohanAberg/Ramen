# Copyright (c) 2011 Esteban Tovagliari
# Licensed under the terms of the CDDL License.
# See CDDL_LICENSE.txt for a copy of the license.

import logging

import PySide.QtCore
import PySide.QtGui

from ramen import r_app

class main_window( PySide.QtGui.QMainWindow):
    def __init__( self, parent = None):
        super( main_window, self).__init__( parent)
        logging.debug( 'Created main window')
        self.setWindowTitle( r_app().full_version_name())
