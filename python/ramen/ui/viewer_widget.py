# Copyright (c) 2011 Esteban Tovagliari
# Licensed under the terms of the CDDL License.
# See CDDL_LICENSE.txt for a copy of the license.

import logging

import PySide.QtCore as QtCore
import PySide.QtGui as QtGui
import PySide.QtOpenGL as QtOpenGL

import OpenGL

class viewer( QtOpenGL.QGLWidget):
    def __init__( self, parent = None):
        super( viewer, self).__init__( parent)

class viewer_widget( QtGui.QWidget):
    def __init__( self, parent = None):
        super( viewer_widget, self).__init__( parent)
        layout = QtGui.QVBoxLayout()

        self.__viewer = viewer()
        layout.addWidget( self.__viewer)

        self.setLayout( layout)
