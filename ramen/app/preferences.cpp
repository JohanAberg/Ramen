// Copyright (c) 2010 Esteban Tovagliari.
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/app/preferences.hpp>

#include<stdlib.h>
#include<stdio.h>

#include<boost/filesystem/fstream.hpp>

#include<ramen/app/application.hpp>

namespace ramen
{

preferences_t::preferences_t()
{
    set_defaults();
    load();
}

void preferences_t::set_defaults()
{
    max_image_memory_ = 30; // % of ram	
	tmp_dir_ = app().system().tmp_path();
}

void preferences_t::load()
{
	// TODO: load pick distance
	
    boost::filesystem::path p = app().system().preferences_path() / "preferences.yaml";
    boost::filesystem::ifstream ifs( p);

    if( !ifs.is_open() || !ifs.good())
    {
        save();
        return;
    }

    try
    {
        YAML::Parser parser( ifs);

        YAML::Node doc;
        parser.GetNextDocument(doc);

        int version;

		if( !get_value( doc, "version", version))
            throw std::runtime_error( "Corrupted preferences file");

        get_value( doc, "max_image_memory", max_image_memory_);
    }
    catch( ...)
    {
        set_defaults();
        save();
    }
}

void preferences_t::save()
{
    boost::filesystem::path p = app().system().preferences_path() / "preferences.yaml";
    boost::filesystem::ofstream ofs( p);

    if( !ofs.is_open() || !ofs.good())
    {
        app().error( "Couldn't open preferences file for writting. file = " + filesystem::file_string( p));
        return;
    }

	// TODO: save pick distance
	
    YAML::Emitter out;
    out << YAML::Comment( "Ramen preferences") << YAML::Newline;

    out << YAML::BeginMap;
        out << YAML::Key << "version" << YAML::Value << 1
            << YAML::Key << "max_image_memory" << YAML::Value << max_image_memory_
            ;

    out << YAML::EndMap;
    ofs << out.c_str();
	std::flush( ofs);
    ofs.close();
}

const boost::filesystem::path& preferences_t::tmp_dir() const { return tmp_dir_;}

} // namespace
