// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/config.hpp>

#include<ramen/python/python.hpp>

#include<boost/python.hpp>

#include<ramen/app/application.hpp>
#include<ramen/version.hpp>

namespace bpy = boost::python;
using namespace ramen;

namespace ramen
{

struct py_application_access
{
    static void create_dirs( application_t *a)
    {
        a->create_dirs();
    }

    static void init_threads( application_t *a, int num)
    {
        a->init_threads( num);
    }

    static void init_ocio( application_t *a)
    {
        a->init_ocio();
    }

    static void print_app_info( application_t *a)
    {
        a->print_app_info();
    }

    static void set_preferences( application_t *a, boost::python::object prefs)
    {
        a->set_preferences( prefs);
    }

    static void run_unit_tests( application_t *a, bpy::list args)
    {
        a->run_unit_tests( 0, 0);
    }

    // version, names, ...

    static int version_major( application_t *a) { return RAMEN_VERSION_MAJOR;}
    static int version_minor( application_t *a) { return RAMEN_VERSION_MINOR;}
    static int version_patch( application_t *a) { return RAMEN_VERSION_PATCH;}

    static const char *full_version_name( application_t *a)
    {
        return RAMEN_NAME_FULL_VERSION_STR;
    }
};

application_t *py_app()
{
    return &app();
}

} // ramen

namespace
{

} // unnamed

void export_application()
{
    bpy::class_<application_t, boost::noncopyable>( "base_application")
        .def( "create_dirs", &py_application_access::create_dirs)

        .def( "init_threads", &py_application_access::init_threads)
        .def( "init_ocio", &py_application_access::init_ocio)

        .def( "system", &application_t::system, bpy::return_value_policy<bpy::reference_existing_object>())

        .def( "print_app_info", &py_application_access::print_app_info)

        .def( "set_preferences", &py_application_access::set_preferences)

        .def( "run_unit_tests", &py_application_access::run_unit_tests)

        .def( "version_major", &py_application_access::version_major)
        .def( "version_minor", &py_application_access::version_minor)
        .def( "version_patch", &py_application_access::version_patch)

        .def( "full_version_name", &py_application_access::full_version_name)
        ;

    bpy::def( "r_app", py_app, bpy::return_value_policy<bpy::reference_existing_object>());
}
