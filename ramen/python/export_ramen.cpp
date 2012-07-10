// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/config.hpp>

#include<boost/python.hpp>

// prototypes
void export_application();
void export_system();

void export_ramen()
{
    export_application();
    export_system();
}

// main python module
BOOST_PYTHON_MODULE( _ramen)
{
    export_ramen();
}
