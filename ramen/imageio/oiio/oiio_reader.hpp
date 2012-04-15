// Copyright (c) 2010 Esteban Tovagliari

#ifndef RAMEN_IMAGEIO_OIIO_READER_HPP
#define	RAMEN_IMAGEIO_OIIO_READER_HPP

#include<ramen/imageio/reader.hpp>

#include<OpenImageIO/imageio.h>
#include<OpenImageIO/typedesc.h>
namespace OIIO = OIIO_NAMESPACE;

namespace ramen
{
namespace imageio
{

class oiio_reader_t : public reader_t
{
public:

    oiio_reader_t( const boost::filesystem::path& p);

private:

    virtual void do_read_image( const image::image_view_t& view, const Imath::Box2i& crop, int subsample) const;
    void do_read_tiled_image( const image::image_view_t& view, const Imath::Box2i& crop, int subsample) const;
	
	void copy_tile( int x, int y, const OIIO::ImageSpec& spec, float *data, 
					   const Imath::Box2i& crop, const image::image_view_t& view, int subsample) const;
	
	bool is_tiled_;
};

} // namespace
} // namespace

#endif
