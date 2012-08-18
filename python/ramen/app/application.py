# Copyright (c) 2011 Esteban Tovagliari
# Licensed under the terms of the CDDL License.
# See CDDL_LICENSE.txt for a copy of the license.

import sys
import logging
import unittest

import shiboken
import PySide.QtCore as QtCore
import PySide.QtGui as QtGui

from ramen import base_application
from ramen.app.preferences import preferences

from ramen.ui import splash_screen
from ramen.ui.user_interface import user_interface

class application( base_application):
    def __init__( self, args):
        super( application, self).__init__()
        logging.debug( 'Created application')

        self.__args = args
        self.__ui = None
        self.__run_tests = False
        self.__splash = None

        self._create_dirs()

        # preferences
        self.__prefs = preferences( self.system().preferences_path())
        self._set_preferences( self.__prefs)

        # command line
        self.__parse_command_line( args)


        logging.debug( 'Creating Splash screen')
        app = QtGui.QApplication( self.__args)
        self.__splash = splash_screen()
        self.__splash.show()

        # init
        logging.debug( 'Initializing threading')
        if self.__splash:
            self.__splash.show_message( "Initializing threading")
        self._init_threads( 0)

        logging.debug( 'Initializing OCIO')
        if self.__splash:
            self.__splash.show_message( "Initializing OCIO")
        self._init_ocio()

    def arguments( self):
        return self.__args

    def preferences( self):
        return self.__prefs

    def run( self):
        result = 0

        if self.__run_tests:
            logging.debug( 'Running unit tests\n')
            result = self.run_all_tests()
            sys.exit( result)

        self._print_app_info()

        self.__ui = user_interface( self)

        if self.__splash:
            self.__splash.finish( shiboken.getCppPointer( self.__ui.window())[0])
            del self.__splash

        return self.__ui.run( self.__args)

    def quit( self):
        pass

    # tests
    def run_all_tests( self):
        logging.debug( 'C++ tests:')
        logging.debug( '----------\n')
        result = self._run_unit_tests( self.__args)

        logging.debug( 'Python tests:')
        logging.debug( '-------------\n')
        sys.argv = [ self.__args[0]]
        suite = unittest.TestLoader().loadTestsFromTestCase( TestApplicationFunctions)
        unittest.TextTestRunner( verbosity=2).run( suite)

        return result

    # command line
    def __parse_command_line( self, args):
        if '--help' in args:
            self.__usage()
            sys.exit( 0)

        if '--version' in args:
            print self.full_version_name()
            sys.exit( 0)

        if '--run-unit-tests' in args:
            self.__run_tests = True

        # todo: do something interesting here

    def __usage( self):
        print "Usage: (TODO)"

#################################################
# Testing

class TestApplicationFunctions( unittest.TestCase):
    def setUp( self):
        pass

    def test_placeholder( self):
        self.assertEqual( 7, 7)
