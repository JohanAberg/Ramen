# Copyright (c) 2012 Esteban Tovagliari
# Licensed under the terms of the CDDL License.
# See CDDL_LICENSE.txt for a copy of the license.

import PySide.QtCore as QtCore
import PySide.QtGui as QtGui

class script_editor_widget( QtGui.QPlainTextEdit):
    def __init__( self, parent = None):
        super( script_editor_widget, self).__init__( parent)

