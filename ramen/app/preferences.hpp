// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_PREFERENCES_HPP
#define RAMEN_PREFERENCES_HPP

#include<ramen/python/python.hpp>

#include<vector>
#include<string>

#include<boost/noncopyable.hpp>

#include<OpenEXR/ImfChromaticities.h>

#include<ramen/app/application_fwd.hpp>

#include<ramen/filesystem/path.hpp>

#include<ramen/serialization/yaml.hpp>

namespace ramen
{

/**
\ingroup app
\brief contains the user preferences
*/
class preferences_t : boost::noncopyable
{
public:

    // memory
    int max_image_memory() const        { return max_image_memory_;}
    void set_max_image_memory( int m)   { max_image_memory_ = m;}

    // paths
    const boost::filesystem::path& tmp_dir() const;

    void save();

private:

    friend class application_t;

    preferences_t();

    void set_defaults();

    void load();

    template<class T>
    bool get_value( const YAML::Node& doc, const std::string& key, T& value)
    {
        if( const YAML::Node *n = doc.FindValue( key))
        {
            *n >> value;
            return true;
        }

        return false;
    }

    int max_image_memory_;
    boost::filesystem::path tmp_dir_;
};

} // namespace

#endif
