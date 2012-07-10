// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_CONFIG_HPP
#define RAMEN_CONFIG_HPP

#include<base/config.hpp>
#include<base/visibility.hpp>

#ifdef ramen_EXPORTS
    #define RAMEN_API BASE_EXPORT
#else
    #define RAMEN_API BASE_IMPORT
#endif

#define RAMEN_FORCEINLINE BASE_FORCEINLINE
#define RAMEN_WARN_UNUSED_RESULT BASE_WARN_UNUSED_RESULT

#endif
