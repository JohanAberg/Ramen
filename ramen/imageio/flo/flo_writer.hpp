// Copyright (c) 2010 Esteban Tovagliari

#ifndef RAMEN_IMAGEIO_FLO_WRITER_HPP
#define RAMEN_IMAGEIO_FLO_WRITER_HPP

#include<ramen/imageio/writer.hpp>

namespace ramen
{
namespace imageio
{

class flo_writer_t : public writer_t
{
public:
    
    flo_writer_t() : writer_t() {}

private:

    virtual void do_write_image( const boost::filesystem::path& p,
				const image::const_image_view_t& view,
				const adobe::dictionary_t& params) const;
};

} // namespace
} // namespace

#endif
