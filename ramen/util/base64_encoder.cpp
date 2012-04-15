// Copyright (c) 2011 Esteban Tovagliari

#include<ramen/util/base64_encoder.hpp>

#include<cctype>

namespace ramen
{

const std::string base64_encoder_t::valid_chars_( "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");
	
base64_encoder_t::base64_encoder_t() {}

bool base64_encoder_t::is_base64( unsigned char c) const
{
	return std::isalnum(c) || (c == '+') || (c == '/');
}

} // ramen
