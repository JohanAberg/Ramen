// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_NODE_FWD_HPP
#define	RAMEN_NODE_FWD_HPP

#include<boost/intrusive_ptr.hpp>

namespace ramen
{

class node_t;
typedef boost::intrusive_ptr<node_t> node_ptr_t;

class image_node_t;

} // namespace

#endif
