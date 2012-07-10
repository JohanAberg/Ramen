// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_CONFIG_HPP
#define RAMEN_UI_CONFIG_HPP

#include<ramen/config.hpp>

#ifdef ramen_ui_EXPORTS
    #define RAMEN_UI_API BASE_EXPORT
#else
    #define RAMEN_UI_API BASE_IMPORT
#endif

#endif
