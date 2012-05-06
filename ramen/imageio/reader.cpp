// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/imageio/reader.hpp>

#include<algorithm>

#include<boost/bind.hpp>

namespace ramen
{
namespace imageio
{
	
reader_t::reader_t( const boost::filesystem::path& p) : path_(p) {}

const adobe::dictionary_t& reader_t::image_info() const { return info_;}

Imath::Box2i reader_t::format() const
{
    return adobe::get_value( info_, adobe::name_t( "format")).cast<Imath::Box2i>();
}

Imath::Box2i reader_t::bounds() const
{
    Imath::Box2i bounds( format());
    adobe::get_value( info_, adobe::name_t( "bounds"), bounds);
    return bounds;
}

float reader_t::aspect_ratio() const
{
	float aspect = 1.0f;
	adobe::get_value( info_, adobe::name_t( "aspect"), aspect);
	return aspect;
}

void reader_t::read_image( const image::image_view_t& view) const
{
    read_image( view, bounds(), 1);
}

void reader_t::read_image( const image::image_view_t& view, const Imath::Box2i& crop, int subsample) const
{
    do_read_image( view, crop, subsample);
}

void reader_t::repeat_scanline_until_end(const image::image_view_t& view, int y) const
{
    int last_scan = y;
    int yy = y + 1;

    while( yy < view.height())
    {
        std::copy( view.row_begin( last_scan), view.row_end( last_scan), view.row_begin( yy));
        ++yy;
    }
}

} // namespace
} // namespace
