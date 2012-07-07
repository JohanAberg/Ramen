// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_VISIBILITY_HPP
#define RAMEN_VISIBILITY_HPP

#include<base/visibility.hpp>

#define RAMEN_EXPORT  BASE_EXPORT
#define RAMEN_IMPORT  BASE_IMPORT
#define RAMEN_PRIVATE BASE_PRIVATE

#ifdef RAMEN_MAKING_DSO
    #define RAMEN_API RAMEN_EXPORT
#else
    #define RAMEN_API RAMEN_IMPORT
#endif

#endif
