// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UNIQUE_NAME_MAP_HPP
#define RAMEN_UNIQUE_NAME_MAP_HPP

#include<map>
#include<string>
#include<stdexcept>

#include<boost/lexical_cast.hpp>

namespace ramen
{
namespace detail
{
	
class unique_name_map_base_t
{
public:
	
	unique_name_map_base_t() {}

protected:
		
	std::string get_number( const std::string& str) const
	{
		try
		{
			// this is a quick fix, if all str are nums
			int num = boost::lexical_cast<int>( str);
			return str;
		}
		catch( ...) {}
	
		std::string::size_type n = str.length() - 1;
	
		while( n != 0)
		{
			char c = str[n];
	
			if( !isdigit( c))
				break;
	
			--n;
		}
	
		return std::string( str, n + 1, str.length());
	}
		
	void increment_string( std::string& str) const
	{
		std::string num_str = get_number( str);
	
		if( num_str.empty())
			str.append( "_2");
		else
		{
			std::string base( str, 0, str.length() - num_str.length());
			int num = boost::lexical_cast<int>( num_str);
			num_str = boost::lexical_cast<std::string>( ++num);
			str = base + num_str;
		}
	}	
};
	
} // detail
	
template<class T>
class unique_name_map_t : public detail::unique_name_map_base_t
{
public:

    unique_name_map_t() : detail::unique_name_map_base_t() {}

    void insert( const T& val)
	{
		std::string name( val.name());

		while( 1)
		{
			typename std::map<std::string, T>::iterator it( map_.find( name));

			if( it != map_.end())
			{
				if( it->second == val)
					return; // value already in the map.
			}
			else
			{
				val.set_name( name);
				map_[name] = val;
				return;
			}
			
			increment_string( name);
		}
	}
	
	void remove( const std::string& name) { map_.erase( name);}
	
    T& find( const std::string& name)
	{
		typename std::map<std::string, T>::iterator it( map_.find( name));
		
		if( it == map_.end())
			throw std::out_of_range( "object not in name map");
		
		return it->second;
	}
	
    std::string make_name_unique( const std::string& n) const
	{
		std::string name( n);

		while( 1)
		{
			typename std::map<std::string, T>::const_iterator it( map_.find( name));

			if( it == map_.end())
				return name;
			else
				increment_string( name);
		}
	}

private:

	std::map<std::string, T> map_;
};

template<class T>
class unique_name_map_t<T*> : public detail::unique_name_map_base_t
{
public:

    unique_name_map_t() {}

    void insert( T *val)
	{
		std::string name( val->name());

		while( 1)
		{
			typename std::map<std::string, T*>::iterator it( map_.find( name));

			if( it != map_.end())
			{
				if( it->second == val)
					return; // value already in the map.
			}
			else
			{
				val->set_name( name);
				map_[ name] = val;
				return;
			}
			
			increment_string( name);
		}
	}
	
	void insert_null( const std::string& name)
	{
		typename std::map<std::string, T*>::iterator it( map_.find( name));
		RAMEN_ASSERT( it == map_.end());
		map_[ name] = 0;
	}

	void remove( const std::string& name) { map_.erase( name);}
	
    T *find( const std::string& name)
	{
		typename std::map<std::string, T*>::iterator it( map_.find( name));
		
		if( it == map_.end())
			return 0;
		
		return it->second;
	}
	
    std::string make_name_unique( const std::string& n) const
	{
		std::string name( n);

		while( 1)
		{
			typename std::map<std::string, T*>::const_iterator it( map_.find( name));

			if( it == map_.end())
				return name;
			else
				increment_string( name);
		}
	}

private:
	
	std::map<std::string, T*> map_;
};

} // namespace

#endif
