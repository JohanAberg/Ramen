// Copyright (c) 2010 Esteban Tovagliari

#ifndef RAMEN_IMAGEIO_IMPORT_MULTICHANNEL_EXR_HPP
#define	RAMEN_IMAGEIO_IMPORT_MULTICHANNEL_EXR_HPP

#include<ramen/filesystem/path.hpp>

namespace ramen
{
namespace imageio
{

void import_multichannel_exr( const boost::filesystem::path& p, bool relative, bool sequence);

} // namespace
} // namespace

#endif

