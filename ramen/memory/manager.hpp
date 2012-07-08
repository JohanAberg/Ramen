// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_MEMORY_MANAGER_HPP
#define RAMEN_MEMORY_MANAGER_HPP

#include<ramen/memory/manager_fwd.hpp>

#include<boost/noncopyable.hpp>
#include<boost/integer.hpp>

#include<memory>

#include<ramen/app/application_fwd.hpp>

namespace ramen
{
namespace memory
{

class manager_t : boost::noncopyable
{
public:

    ~manager_t();

private:

    friend class ramen::application_t;

    explicit manager_t( boost::uint64_t img_cache_size);
};

} // namespace
} // namespace

#endif
