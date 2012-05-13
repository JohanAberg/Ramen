// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/assert.hpp>

#include<cstdlib>
#include<iostream>

namespace ramen
{

void assertion_failed( char const *expr, char const *function, char const *file, long line)
{
	std::cerr << "assertion failed, file: " << file << " , function: " << function  << " " << line << ": " << expr << "\n";
	std::abort();
}

} // boost
