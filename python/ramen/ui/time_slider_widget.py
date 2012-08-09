# Copyright (c) 2012 Esteban Tovagliari
# Licensed under the terms of the CDDL License.
# See CDDL_LICENSE.txt for a copy of the license.

import logging
import math

import PySide.QtCore as QtCore
import PySide.QtGui as QtGui

from ramen.ui.nice_numbers import nice_numbers

class time_scale_widget( QtGui.QWidget):
    def __init__( self, parent = None):
        super( time_scale_widget, self).__init__( parent)
        self.__min = 1
        self.__max = 100
        self.__val = 1

    def set_range( self, lo, hi):
        self.__min = lo
        self.__max = hi
        self.__clamp_value()

    def set_value( self, val):
        self.__val = val
        self.__clamp_value()

    def paintEvent( self, event):
        painter = QtGui.QPainter( self)
        painter.setRenderHint( QtGui.QPainter.Antialiasing)

        pen = QtGui.QPen()
        pen.setColor( QtGui.QColor( 0, 0, 0))
        pen.setWidth( 1)
        painter.setPen( pen)
        painter.drawLine( 0, 7, self.width(), 7)

        spacing = 50.0;
        nticks = math.floor( self.width() / spacing)

        for x in nice_numbers( self.__min, self.__max, nticks):
            sx = float( x - self.__min) / float( self.__max - self.__min) * self.width()
            painter.drawLine( QtCore.QPointF( sx, 2), QtCore.QPointF( sx, 12))
            painter.drawText( QtCore.QPoint( sx, self.height()), str( int( x)))

        pen.setColor( QtGui.QColor( 255, 0, 0))
        pen.setWidth( 3)
        painter.setPen( pen)

        x = float(self.__val - self.__min) / float( self.__max - self.__min) * self.width()
        painter.drawLine( QtCore.QPointF( x, 0), QtCore.QPointF( x, self.height()))
        event.accept()

    def __clamp_value( self):
        if self.__val < self.__min:
            self.__val = self.__min

        if self.__val > self.__max:
            self.__val = self.__max

class time_slider_widget( QtGui.QWidget):
    def __init__( self, parent = None):
        super( time_slider_widget, self).__init__( parent)

        start_ = QtGui.QDoubleSpinBox()
        #start_.setTrackMouse( False)
        start_.setRange( -32768, 32768)
        start_.setValue( 1)
        start_.setSizePolicy( QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)

        end_ = QtGui.QDoubleSpinBox()
        #end_.setTrackMouse( False)
        end_.setRange( -32768, 32768)
        end_.setValue( 100)
        end_.setSizePolicy( QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)

        current_ = QtGui.QDoubleSpinBox()
        #current_.setTrackMouse( False)
        current_.setRange(1, 100)
        current_.setValue( 1)
        current_.setSizePolicy( QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)

        scale_ = time_scale_widget()
        scale_.set_range(1, 100)
        scale_.set_value( 1)

        #connect( start_    , SIGNAL( valueChanged(double)), this, SLOT( set_start_frame(double)))
        #connect( end_		, SIGNAL( valueChanged(double)), this, SLOT( set_end_frame(double)))
        #connect( current_	, SIGNAL( valueChanged(double)), this, SLOT( set_frame(double)))
        #connect( scale_    , SIGNAL( valueChanged(double)), this, SLOT( set_frame(double)))

        layout = QtGui.QHBoxLayout()
        layout.addWidget( start_)
        layout.addWidget( current_)
        layout.addWidget( scale_)
        layout.addWidget( end_)
        self.setLayout( layout)
        self.setSizePolicy( QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Fixed)
