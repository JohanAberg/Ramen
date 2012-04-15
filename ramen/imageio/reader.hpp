// Copyright (c) 2010 Esteban Tovagliari

#ifndef RAMEN_IMAGEIO_READER_HPP
#define	RAMEN_IMAGEIO_READER_HPP

#include<vector>
#include<string>

#include<boost/cstdint.hpp>

#include<OpenEXR/ImathBox.h>

#include<adobe/dictionary.hpp>

#include<ramen/assert.hpp>

#include<ramen/filesystem/path.hpp>

#include<ramen/imageio/exceptions.hpp>
#include<ramen/imageio/enums.hpp>

#include<ramen/image/typedefs.hpp>

namespace ramen
{
namespace imageio
{

class RAMEN_API reader_t
{
public:

    reader_t( const boost::filesystem::path& p);
    virtual ~reader_t() {}

    const adobe::dictionary_t& image_info() const;

    Imath::Box2i format() const;
    Imath::Box2i bounds() const;

	float aspect_ratio() const;
	
    void read_image( const image::image_view_t& view) const;

    void read_image( const image::image_view_t& view, const Imath::Box2i& crop, int subsample) const;

private:

    virtual void do_read_image( const image::image_view_t& view, const Imath::Box2i& crop, int subsample) const = 0;
	
protected:

    void repeat_scanline_until_end( const image::image_view_t& view, int y) const;
		
    boost::filesystem::path path_;
    adobe::dictionary_t info_;
};

} // namespace
} // namespace

#endif
