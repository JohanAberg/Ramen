# Copyright (c) 2011 Esteban Tovagliari
# Licensed under the terms of the CDDL License.
# See CDDL_LICENSE.txt for a copy of the license.

import logging

from ramen import init_pyside_interop
from ramen.app.application import application

def main( args):
    logging.debug( 'Starting Ramen')

    logging.debug( 'Initializing PySide C++ interop')
    init_pyside_interop()

    rapp = application( args)
    result = rapp.run()
    return result
