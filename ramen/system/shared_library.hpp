// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_SYSTEM_SHARED_LIBRARY_HPP
#define RAMEN_SYSTEM_SHARED_LIBRARY_HPP

#include<string>

#include<boost/function.hpp>

#include<boost/filesystem/fstream.hpp>

namespace ramen
{
namespace system
{

class shared_library_t
{
public:

    shared_library_t( const boost::filesystem::path& p);
    ~shared_library_t();

    template<class S>
    boost::function<S> get_function( const std::string& name) const
    {
        return boost::function<S>();
    }

private:

};

} // namespace
} // namespace

#endif
