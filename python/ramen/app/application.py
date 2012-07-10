# Copyright (c) 2011 Esteban Tovagliari
# Licensed under the terms of the CDDL License.
# See CDDL_LICENSE.txt for a copy of the license.

import sys
import logging

from ramen import base_application

from ramen.app.preferences import preferences
from ramen.ui.user_interface import user_interface

class application( base_application):
    def __init__( self, args):
        super( application, self).__init__()
        logging.debug( 'Created application')

        self._args = args

        self._ui = None
        self._run_tests = False

        #self.create_dirs()

        # preferences
        self._prefs = preferences( self.system().preferences_path())
        self.set_preferences( self._prefs)

        # command line
        self.parse_command_line( args)

        # init
        self.init_threads( 0)
        self.init_ocio()


    def run( self):
        result = 0

        if self._run_tests:
            logging.debug( 'Running unit tests')
            self.run_all_tests()
            sys.exit( 0)

        self.print_app_info()

        self._ui = user_interface()
        result = self._ui.run( self._args)

        self.quit()
        return result

    def quit( self):
        pass

    # command line
    def parse_command_line( self, args):
        if '--help' in args:
            self.usage()
            sys.exit( 0)

        if '--version' in args:
            print self.full_version_name()
            sys.exit( 0)

        if '--run-unit-tests' in args:
            self._run_tests = True

        # todo: do something interesting here

    def usage( self):
        print "Usage: (TODO)"

    # tests
    def run_all_tests( self):
        self.run_unit_tests( self._args)
