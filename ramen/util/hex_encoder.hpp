// Copyright (c) 2011 Esteban Tovagliari

#ifndef RAMEN_HEX_ENCODER_HPP
#define RAMEN_HEX_ENCODER_HPP

#include<string>
#include<algorithm>

#include<boost/range.hpp>

namespace ramen
{

class hex_encoder_t
{
public:
	
	hex_encoder_t();
	
	template<class IRange, class OutCont>
	void encode( const IRange& input, OutCont& output) const
	{
		typedef BOOST_DEDUCED_TYPENAME boost::range_iterator<IRange>::type iter_type;
		iter_type it( boost::begin( input));

		output.clear();
		output.resize( boost::size( input) * 2);
		
		for( std::size_t i = 0; i < boost::size( input); ++i)
			char_to_hex( it[i], output[i*2+0], output[i*2+1]);
	}

	template<class IRange, class OutCont>
	void decode( const IRange& input, OutCont& output) const
	{
		typedef BOOST_DEDUCED_TYPENAME boost::range_iterator<IRange>::type iter_type;
		iter_type it( boost::begin( input));
		
		output.clear();
		output.resize( boost::size( input) / 2);
		
		for (std::size_t i = 0; i < output.size(); ++i)
			output[i] = hex_to_char( it[i*2+0], it[i*2+1]);
	}

private:

	inline void char_to_hex( unsigned char c, char& hi, char& low)
	{
		low = c & 0x0f;
		hi = (c >> 4) & 0x0f;
		
		low += (low >= 10 ? 'a' - 10 : '0');
		hi += (hi >= 10 ? 'a' - 10 : '0');
	}
	
	inline unsigned char hex_to_char( char hi, char low)
	{
		unsigned char r = 0;
		
		if (low >= 'a' && low <= 'f') r |= ((low - 'a' + 10) & 0x0f);
		else if (low >= 'A' && low <= 'F') r |= ((low - 'A' + 10) & 0x0f);
		else if (low >= '0' && low <= '9') r |= ((low - '0') & 0x0f);

		if (hi >= 'a' && hi <= 'f') r |= (((hi - 'a' + 10) & 0x0f) << 4);
		else if (hi >= 'A' && hi <= 'F') r |= (((hi - 'A' + 10) & 0x0f) << 4);
		else if (hi >= '0' && hi <= '9') r |= (((hi - '0') & 0x0f) << 4);
		return r;
	}
	
};
	
} // ramen

#endif
