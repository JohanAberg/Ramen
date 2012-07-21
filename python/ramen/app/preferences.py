# Copyright (c) 2011 Esteban Tovagliari
# Licensed under the terms of the CDDL License.
# See CDDL_LICENSE.txt for a copy of the license.

import logging
import yaml

class preferences( object):
    def __init__( self, path):
        logging.debug( 'Creating preferences: path = %s\n', path)
        self._path = path

    def load( self):
        pass

    def save( self):
        pass
