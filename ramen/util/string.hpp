// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UTIL_STRING_HPP
#define RAMEN_UTIL_STRING_HPP

#include<string>

namespace ramen
{
namespace util
{

bool is_string_valid_identifier( const std::string& str);

void increment_string_number( std::string& str);

std::string make_string( const char *s0, const char *s1, const char *s2 = 0, const char *s3 = 0);

std::string concat_strings( const std::string& a, const std::string& b, const std::string& c = std::string());

} // util
} // ramen

#endif
