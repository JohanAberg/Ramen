// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_FILESYSTEM_PATH_HPP
#define RAMEN_FILESYSTEM_PATH_HPP

#include<ramen/config.hpp>

#include<boost/filesystem/path.hpp>

namespace ramen
{
namespace filesystem
{

RAMEN_API const std::string& file_string( const boost::filesystem::path& p);
RAMEN_API const char *file_cstring( const boost::filesystem::path& p);

RAMEN_API std::string hash_string( const boost::filesystem::path& p);

RAMEN_API boost::filesystem::path make_absolute_path( const boost::filesystem::path& p, const boost::filesystem::path& from);
RAMEN_API boost::filesystem::path make_relative_path( const boost::filesystem::path& p, const boost::filesystem::path& from);
RAMEN_API boost::filesystem::path convert_relative_path( const boost::filesystem::path& p, const boost::filesystem::path& old_base,
                                                          const boost::filesystem::path& new_base);

} // namespace
} // namespace

#endif
