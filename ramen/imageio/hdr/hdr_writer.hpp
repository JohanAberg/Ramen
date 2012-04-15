// Copyright (c) 2010 Esteban Tovagliari

#ifndef RAMEN_IMAGEIO_HDR_WRITER_HPP
#define RAMEN_IMAGEIO_HDR_WRITER_HPP

#include<ramen/imageio/oiio/oiio_writer.hpp>

namespace ramen
{
namespace imageio
{

class hdr_writer_t : public oiio_writer_t
{
public:
    
    hdr_writer_t() : oiio_writer_t() {}

private:

    virtual void do_write_image( const boost::filesystem::path& p,
				const image::const_image_view_t& view,
				const adobe::dictionary_t& params) const;
};

} // namespace
} // namespace

#endif
