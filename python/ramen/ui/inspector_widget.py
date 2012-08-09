# Copyright (c) 2011 Esteban Tovagliari
# Licensed under the terms of the CDDL License.
# See CDDL_LICENSE.txt for a copy of the license.

import logging

import PySide.QtCore as QtCore
import PySide.QtGui as QtGui

class inspector_widget( QtGui.QWidget):
    def __init__( self, parent = None):
        super( inspector_widget, self).__init__( parent)

        self.__width = -1
        self.__left_margin = -1

        layout = QtGui.QVBoxLayout()
        layout.setContentsMargins( 0, 0, 0, 0)

        self.setSizePolicy( QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Preferred)
        self.setMinimumSize( self.width() + 16, 0)
        self.setMaximumSize( self.width() + 16, 65535)
        self.setWindowTitle( "Inspector")
        self.setContentsMargins( 0, 0, 0, 0)

        scroll_ = QtGui.QScrollArea()
        scroll_.setVerticalScrollBarPolicy( QtCore.Qt.ScrollBarAsNeeded)
        scroll_.setHorizontalScrollBarPolicy( QtCore.Qt.ScrollBarAlwaysOff)
        scroll_.setWidgetResizable( True)
        scroll_.setContentsMargins( 0, 0, 0, 0)

        layout2 = QtGui.QVBoxLayout()
        layout2.setContentsMargins( 0, 0, 0, 0)

        top = QtGui.QWidget()
        layout2.addWidget( self.__create_header())

        separator = QtGui.QFrame()
        separator.setFrameStyle( QtGui.QFrame.HLine | QtGui.QFrame.Raised)
        separator.setLineWidth( 1)
        layout2.addWidget( separator)

        view_ = QtGui.QWidget()
        layout2.addWidget( view_)
        top.setLayout( layout2)
        scroll_.setWidget( top)

        layout.addWidget( scroll_)
        self.setLayout( layout)

    def left_margin( self):
        if self.__left_margin < 0:
            tmp = QtGui.QLabel()
            tmp.setText( "M")
            s = tmp.sizeHint()
            del tmp
            max_label_lenght = 16;
            self.__left_margin = s.width() * max_label_lenght;

        return self.__left_margin

    def width( self):
        if self.__width < 0:
            tmp = QtGui.QSpinBox()
            s = tmp.sizeHint()
            del tmp
            self.__width = self.left_margin() + 5 + ( 3 * s.height()) + ( 3 * s.width()) + 30

        return self.__width

    def __create_header( self):
        header = QtGui.QWidget()
        self.__name_edit = QtGui.QLineEdit( header)
        self.__help = QtGui.QPushButton( header)
        self.__help.setText( "Help")
        s = self.__help.sizeHint()

        hpos = self.left_margin() / 2;
        vsize = self.__name_edit.sizeHint().height()

        label = QtGui.QLabel( header)
        label.setText( "Name")
        label.move( 0, 5)
        label.resize( hpos - 5, vsize)
        label.setAlignment( QtCore.Qt.AlignRight | QtCore.Qt.AlignVCenter)

        self.__name_edit.move( hpos, 5)
        self.__name_edit.resize( self.width() - hpos - 15 - s.width() - 5, vsize)
        self.__name_edit.setEnabled( False)

        #connect( self.__name_edit, SIGNAL( editingFinished()), this, SLOT( rename_node()))
        hpos = hpos + self.width() - hpos - 15 - s.width()

    	self.__help.move( hpos, 5)
        self.__help.setEnabled( False)
        #connect( self.__help, SIGNAL( clicked()), this, SLOT( show_help()))

        header.setMinimumSize( self.width(), vsize + 10)
        header.setMaximumSize( self.width(), vsize + 10)
        header.setSizePolicy( QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)
        return header
