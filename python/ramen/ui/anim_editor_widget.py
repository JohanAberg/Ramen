# Copyright (c) 2011 Esteban Tovagliari
# Licensed under the terms of the CDDL License.
# See CDDL_LICENSE.txt for a copy of the license.

import logging

import PySide.QtCore as QtCore
import PySide.QtGui as QtGui

class anim_editor_widget( QtGui.QWidget):
    def __init__( self, parent = None):
        super( anim_editor_widget, self).__init__( parent)
