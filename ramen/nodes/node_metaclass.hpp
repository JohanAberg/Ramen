// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_NODE_METACLASS_HPP
#define RAMEN_NODE_METACLASS_HPP

#include<ramen/config.hpp>

#include<ramen/nodes/node_fwd.hpp>

#include<string>

#include<boost/function.hpp>

namespace ramen
{

class RAMEN_API node_metaclass_t
{
public:

    node_metaclass_t();

    // unique id and versions
    std::string id;
    int major_version;
    int minor_version;

    // ui related
    bool ui_visible;

    std::string menu;
    std::string submenu;
    std::string menu_item;
    
    std::string hotkey;
    const char *help;

	// init & cleanup
	boost::function<void ()> init;
	boost::function<void ()> cleanup;
	
    // factory
    boost::function<node_t*()> create;
    boost::function<node_t*()> create_gui;

// private:
	
	bool first_time_;
};

} // namespace

#endif
