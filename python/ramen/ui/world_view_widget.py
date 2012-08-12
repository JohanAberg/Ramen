# Copyright (c) 2012 Esteban Tovagliari
# Licensed under the terms of the CDDL License.
# See CDDL_LICENSE.txt for a copy of the license.

import logging

import PySide.QtCore as QtCore
import PySide.QtGui as QtGui

class world_view( QtGui.QWidget):
    def __init__( self, parent = None):
        super( world_view, self).__init__( parent)


class world_view_widget( QtGui.QWidget):
    def __init__( self, parent = None):
        super( world_view_widget, self).__init__( parent)
        layout = QtGui.QVBoxLayout()
        self.__view = world_view()
        layout.addWidget( self.__view)
        self.setLayout( layout)
