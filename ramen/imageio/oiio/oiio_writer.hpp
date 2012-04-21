// Copyright (c) 2010 Esteban Tovagliari

#ifndef RAMEN_IMAGEIO_OIIO_WRITER_HPP
#define RAMEN_IMAGEIO_OIIO_WRITER_HPP

#include<ramen/imageio/writer.hpp>

#include<OpenImageIO/imageio.h>
#include<OpenImageIO/typedesc.h>

#include<memory>

namespace ramen
{
namespace imageio
{

class oiio_writer_t : public writer_t
{
public:
    
    oiio_writer_t() : writer_t() {}

protected:

    void do_write( const boost::filesystem::path& p, const OIIO::ImageSpec& spec,
					 const image::const_image_view_t& view);
	
	void add_common_attributes( OIIO::ImageSpec& spec) const;
};

} // namespace
} // namespace

#endif
