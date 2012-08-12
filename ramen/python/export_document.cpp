// Copyright (c) 2012 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/config.hpp>

#include<ramen/python/python.hpp>

#include<boost/python.hpp>

#include<ramen/app/document.hpp>

namespace bpy = boost::python;
using namespace ramen;

namespace
{

std::string document_get_file( document_t *d)
{
    return d->file().string();
}

} // unnamed

void export_document()
{
    bpy::class_<document_t, bpy::bases<nodes::world_node_t>, boost::noncopyable>( "document", bpy::no_init)
        .def( "dirty", &document_t::dirty)
        .def( "set_dirty", &document_t::set_dirty)
        .def( "has_file", &document_t::has_file)
        .def( "file", document_get_file)
        ;
}
