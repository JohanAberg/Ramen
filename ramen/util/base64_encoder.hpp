// Copyright (c) 2011 Esteban Tovagliari

#ifndef RAMEN_BASE64_ENCODER_HPP
#define RAMEN_BASE64_ENCODER_HPP

#include<string>
#include<algorithm>

#include<boost/range.hpp>

namespace ramen
{

class base64_encoder_t
{
public:
	
	base64_encoder_t();
	
	template<class IRange, class OutCont>
	void encode( const IRange& input, OutCont& output) const
	{
		typedef BOOST_DEDUCED_TYPENAME boost::range_iterator<IRange>::type iter_type;
				
		unsigned char char_array_3[3] = {0};
		unsigned char char_array_4[4] = {0};
		int i = 0;
			
		output.clear();
			
		iter_type first = boost::begin( input);
	
		while( first != boost::end( input))
		{
			char_array_3[i++] = *first++;
				
			if (i == 3)
			{
				char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
				char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
				char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
				char_array_4[3] = char_array_3[2] & 0x3f;
	
				for(int j = 0; (j <4) ; j++)
					output.push_back( valid_chars_[ char_array_4[ j]]);

				i = 0;
			}
		}
	
		if (i)
		{
			std::fill( char_array_3 + i, char_array_3 + 3, '\0');
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;
	
			for (int j = 0; (j < i + 1); j++)
				output.push_back( valid_chars_[ char_array_4[ j]]);

			while( i++ < 3)
				output.push_back( '=');
		}
	}

	template<class IRange, class OutCont>
	void decode( const IRange& input, OutCont& output) const
	{
		typedef BOOST_DEDUCED_TYPENAME boost::range_iterator<IRange>::type iter_type;
		
		unsigned char char_array_4[4] = {0};
		unsigned char char_array_3[3] = {0};
		int i = 0;
		
		output.clear();
		
		// we are sure that we'll be smaller, but at the same time we're not that larger -
		// so there is a good chance that we'll not change the size to the correct one 
		// once the processing is finished
		// dataout.reserve(datain.length());

		iter_type it( boost::begin( input));
		
		while (it != boost::end( input) && *it != '=' && is_base64( *it))
		{
			char_array_4[i++] = *it++;
			
			if (i == 4) 
			{
				for (int j = 0; j < 4; j++)
					char_array_4[j] = valid_chars_.find( char_array_4[j]);

				char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
				char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
				char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

				for (int j = 0; j < 3; j++)
					output.push_back( char_array_3[j]);

				i = 0;
			}
		}
		
		if (i) 
		{
			std::fill( char_array_4 + i, char_array_4 + 4, 0);

			for( int j = 0; j <4; j++)
				char_array_4[j] = valid_chars_.find( char_array_4[j]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (int j = 0; (j < i - 1); j++) 
				output.push_back( char_array_3[j]);
		}
	}
	
private:

	bool is_base64( unsigned char c) const;
	
	static const std::string valid_chars_;
};
	
} // ramen

#endif
