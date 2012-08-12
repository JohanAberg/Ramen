# Copyright (c) 2011 Esteban Tovagliari
# Licensed under the terms of the CDDL License.
# See CDDL_LICENSE.txt for a copy of the license.

import logging

import PySide.QtCore as QtCore
import PySide.QtGui as QtGui

class viewer_tab_widget( QtGui.QWidget):
    def __init__( self, parent = None):
        super( viewer_tab_widget, self).__init__( parent)
        layout = QtGui.QVBoxLayout()
        self.__tabs = QtGui.QTabWidget()
        self.__tabs.setMovable( True)
        self.__tabs.setTabsClosable( True)
        self.__tabs.addTab( QtGui.QWidget(), "Viewer")
        layout.addWidget( self.__tabs)
        self.setLayout( layout)

class viewer_container_widget( QtGui.QWidget):
    def __init__( self, parent = None):
        super( viewer_container_widget, self).__init__( parent)
        layout = QtGui.QVBoxLayout()

        self.__menu_bar = QtGui.QMenuBar()
        self.__view_menu = self.__menu_bar.addMenu( 'View')
        layout.addWidget( self.__menu_bar)

        layout.addWidget( viewer_tab_widget())
        self.setLayout( layout)
