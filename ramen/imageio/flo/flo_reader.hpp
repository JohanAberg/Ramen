// Copyright (c) 2010 Esteban Tovagliari

#ifndef RAMEN_IMAGEIO_FLO_READER_HPP
#define	RAMEN_IMAGEIO_FLO_READER_HPP

#include<ramen/imageio/reader.hpp>

namespace ramen
{
namespace imageio
{

class flo_reader_t : public reader_t
{
public:

    flo_reader_t( const boost::filesystem::path& p);

private:

    virtual void do_read_image( const image::image_view_t& view, const Imath::Box2i& crop, int subsample) const;
};

} // namespace
} // namespace

#endif
