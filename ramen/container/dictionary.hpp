
#ifndef RAMEN_CONTAINER_DICTIONARY_HPP
#define RAMEN_CONTAINER_DICTIONARY_HPP

#include<boost/container/flat_map.hpp>

#include<ramen/name.hpp>
#include<ramen/poly_regular.hpp>

#include<adobe/dictionary.hpp>

namespace ramen
{
namespace container
{

typedef boost::container::flat_map<name_t, poly_regular_t> dictionary_t;

template <typename T>
inline bool get_value( const dictionary_t& dict, name_t key, T& value)
{
    dictionary_t::const_iterator i = dict.find( key);

    if( i == dict.end())
        return false;

    return i->second.cast(value);
}

inline bool get_value( const dictionary_t& dict, name_t key, poly_regular_t& value)
{
    dictionary_t::const_iterator i = dict.find( key);

    if( i == dict.end())
        return false;

    value = i->second;
    return true;
}

inline const poly_regular_t& get_value( const dictionary_t& dict, name_t key)
{
    dictionary_t::const_iterator i = dict.find(key);

    if( i == dict.end())
    {
        std::string error = "dictionary_t: key '";
        error += key.c_str();
        error += "' not found";
        throw std::out_of_range( error);
    }

    return i->second;
}

} // container
} // ramen

#endif
