# Copyright (c) 2011 Esteban Tovagliari
# Licensed under the terms of the CDDL License.
# See CDDL_LICENSE.txt for a copy of the license.

import logging
import weakref

import PySide.QtCore as QtCore
import PySide.QtGui as QtGui

from ramen import base_user_interface
from ramen.ui.main_window import main_window

class user_interface( base_user_interface):
    def __init__( self, app):
        super( user_interface, self).__init__()
        self.__app = weakref.ref( app)
        self.__app()._set_ui( self)
        logging.debug( 'Created user interface')

        if not QtGui.qApp:
            app = QtGui.QApplication( self.__app().arguments())

        self.__main_window = main_window( self.__app(), self)
        self.__restore_window_state()

    def resources_path( self):
        return self.__app().system().resources_path()

    def run( self, args):
        self.create_new_document()
        self.__main_window.show()
        return QtGui.qApp.exec_()

    def quit( self):
        logging.debug( 'Application exiting')
        self.__save_window_state()
        self.__app().quit()
        self.__main_window.deleteLater()
        QtGui.qApp.quit()
        logging.debug( 'Application finished')

    def window( self):
        return self.__main_window

    def update_ui( self):
        self.__main_window.update_ui()

    def create_new_document( self):
        self.__app().create_new_document()
        self.update_ui()

    # errors & messages
    def fatal_error( self, msg):
        # TODO: implement
        assert False

    def error( self, msg):
        # TODO: implement
        assert False

    def inform( self, msg):
        # TODO: implement
        assert False

    def question( self, what, default_answer):
        # TODO: implement
        assert False
        return default_answer

    def __restore_window_state( self):
        path = self.__app().system().preferences_path() + "/wstate.ui"
        file = QtCore.QFile( path)

        if file.open( QtCore.QIODevice.ReadOnly):
            window_state = file.readAll()
            self.__main_window.restoreState( window_state)
            logging.debug( 'Restored window state')

    def __save_window_state( self):
        window_state = self.__main_window.saveState()
        path = self.__app().system().preferences_path() + "/wstate.ui"
        file = QtCore.QFile( path)

        if file.open( QtCore.QIODevice.WriteOnly):
            file.write( window_state)
            logging.debug( 'Saved window state')
