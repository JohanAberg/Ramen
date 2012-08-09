# Copyright (c) 2012 Esteban Tovagliari
# Licensed under the terms of the CDDL License.
# See CDDL_LICENSE.txt for a copy of the license.

import logging

import PySide.QtCore as QtCore
import PySide.QtGui as QtGui

class time_controls_widget( QtGui.QWidget):
    def __init__( self, resources_path, parent = None):
        super( time_controls_widget, self).__init__( parent)

        self.setWindowTitle( "Time Controls")
        self.setContentsMargins( 0, 0, 0, 0)

        layout = QtGui.QHBoxLayout( self)
        layout.setSpacing( 0)

        start_ = QtGui.QToolButton()
        start_.setIcon( QtGui.QIcon( resources_path + "/icons/rewind.png"))
        start_.setFocusPolicy( QtCore.Qt.NoFocus)
        #connect( start_, SIGNAL( pressed()), this, SLOT( goto_start()))
        layout.addWidget( start_)

        prev_key_ = QtGui.QToolButton()
        prev_key_.setIcon( QtGui.QIcon( resources_path + "/icons/prev_key.png"))
        prev_key_.setFocusPolicy( QtCore.Qt.NoFocus)
        prev_key_.setEnabled( False)
        #connect( prev_key_, SIGNAL( pressed()), this, SLOT( prev_key()))
        layout.addWidget( prev_key_)

        prev_frame_ = QtGui.QToolButton()
        prev_frame_.setIcon( QtGui.QIcon( resources_path + "/icons/prev_frame.png"))
        prev_frame_.setFocusPolicy( QtCore.Qt.NoFocus)
        #connect( prev_frame_, SIGNAL( pressed()), this, SLOT( prev_frame()))
        layout.addWidget( prev_frame_)

        play_back_ = QtGui.QToolButton()
        play_back_.setIcon( QtGui.QIcon( resources_path + "/icons/play_back.png"))
        play_back_.setFocusPolicy( QtCore.Qt.NoFocus)
        play_back_.setCheckable( True)
        #connect( play_back_, SIGNAL( pressed()), this, SLOT( play_back()))
        layout.addWidget( play_back_)

        stop_ = QtGui.QToolButton()
        stop_.setIcon( QtGui.QIcon( resources_path + "/icons/stop.png"))
        stop_.setFocusPolicy( QtCore.Qt.NoFocus)
        layout.addWidget( stop_)

        play_fwd_ = QtGui.QToolButton()
        play_fwd_.setIcon( QtGui.QIcon( resources_path + "/icons/play.png"))
        play_fwd_.setFocusPolicy( QtCore.Qt.NoFocus)
        play_fwd_.setCheckable( True)
        #connect( play_fwd_, SIGNAL( pressed()), this, SLOT( play_fwd()))
        layout.addWidget( play_fwd_)

        next_frame_ = QtGui.QToolButton()
        next_frame_.setIcon( QtGui.QIcon( resources_path + "/icons/next_frame.png"))
        next_frame_.setFocusPolicy( QtCore.Qt.NoFocus)
        #connect( next_frame_, SIGNAL( pressed()), this, SLOT( next_frame()))
        layout.addWidget( next_frame_)

        next_key_ = QtGui.QToolButton()
        next_key_.setIcon( QtGui.QIcon( resources_path + "/icons/next_key.png"))
        next_key_.setFocusPolicy( QtCore.Qt.NoFocus)
        next_key_.setEnabled( False)
        #connect( next_key_, SIGNAL( pressed()), this, SLOT( next_key()))
        layout.addWidget( next_key_)

        end_ = QtGui.QToolButton()
        end_.setIcon( QtGui.QIcon( resources_path + "/icons/fastfwd.png"))
        end_.setFocusPolicy( QtCore.Qt.NoFocus)
        #connect( end_, SIGNAL( pressed()), this, SLOT( goto_end()))
        layout.addWidget( end_)

        self.setSizePolicy( QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)
