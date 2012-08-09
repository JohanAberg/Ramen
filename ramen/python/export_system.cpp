// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/config.hpp>

#include<ramen/python/python.hpp>

#include<boost/python.hpp>

#include<ramen/system/system.hpp>
#include<ramen/filesystem/path.hpp>

namespace bpy = boost::python;
using namespace ramen;
using namespace ramen::system;

namespace
{

std::string home_path( const system_t *s)
{
    return filesystem::file_string( s->home_path());
}

std::string app_user_path( const system_t *s)
{
    return filesystem::file_string( s->app_user_path());
}

std::string executable_path( const system_t *s)
{
    return filesystem::file_string( s->executable_path());
}

std::string app_bundle_path( const system_t *s)
{
    return filesystem::file_string( s->app_bundle_path());
}

std::string preferences_path( const system_t *s)
{
    return filesystem::file_string( s->preferences_path());
}

std::string tmp_path( const system_t *s)
{
    return filesystem::file_string( s->tmp_path());
}

std::string persistent_tmp_path( const system_t *s)
{
    return filesystem::file_string( s->persistent_tmp_path());
}

std::string resources_path( const system_t *s)
{
    return filesystem::file_string( s->resources_path());
}

std::string cpu_type( const system_t *s)
{
    switch( s->cpu_type())
    {
        case cpu_x86:
            return "x86";

        default:
            return "Unknown";
    }
}

std::string simd_type( const system_t *s)
{
    switch( s->simd_type())
    {
        case simd_sse:
            return "SSE";

        case simd_sse2:
            return "SSE2";

        case simd_sse3:
            return "SSE3";

        default:
            return "none";
    }
}

} // unnamed

void export_system()
{
    bpy::class_<system_t, boost::noncopyable>( "system")
        .def( "system_name", &system_t::system_name, bpy::return_value_policy<bpy::reference_existing_object>())
        .def( "user_name", &system_t::user_name, bpy::return_value_policy<bpy::reference_existing_object>())

        .def( "home_path", home_path)
        .def( "app_user_path", app_user_path)
        .def( "executable_path", executable_path)
        .def( "app_bundle_path", app_bundle_path)
        .def( "preferences_path", preferences_path)
        .def( "tmp_path", tmp_path)
        .def( "persistent_tmp_path", persistent_tmp_path)
        .def( "resources_path", resources_path)

        .def( "cpu_type", cpu_type)
        .def( "simd_type", simd_type)

        .def( "ram_size", &system_t::ram_size)
        ;
}
