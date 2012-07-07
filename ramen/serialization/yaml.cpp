// Copyright (c) 2010 Esteban Tovagliari

#include<ramen/serialization/yaml.hpp>

#include<ramen/assert.hpp>

#ifndef NDEBUG
    #include<iostream>
#endif

namespace ramen
{

void check_yaml_errors( const YAML::Emitter& out)
{
    #ifndef NDEBUG
        if( !out.good())
        {
            std::cout << "YAML error " << out.GetLastError() << "\n";
            RAMEN_ASSERT( 0);
        }
    #endif
}

const YAML::Node& get_node( const YAML::Node& node, const std::string& key)
{
    return node[key];
}

const YAML::Node *get_optional_node( const YAML::Node& node, const std::string& key)
{
    return node.FindValue( key);
}

void operator>>( const YAML::Node& in, base::empty_t& x)
{
    RAMEN_ASSERT( 0 && "Trying to read an empty_t from a YAML::Node");
}

YAML::Emitter& operator<<( YAML::Emitter& out, const base::empty_t& x)
{
    RAMEN_ASSERT( 0 && "Trying to write an empty_t to a YAML::Emitter");
    return out;
}

void operator>>( const YAML::Node& in, boost::filesystem::path& p)
{
    std::string tmp;
    in >> tmp;
    p = boost::filesystem::path( tmp);
}

YAML::Emitter& operator<<( YAML::Emitter& out, const boost::filesystem::path& p)
{
    out << YAML::DoubleQuoted << filesystem::file_string( p);
    return out;
}

} // namespace
