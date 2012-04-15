// Copyright (c) 2010 Esteban Tovagliari

#include<ramen/util/error_log.hpp>

namespace ramen
{

error_log_t::error_log_t() {}

std::stringstream& error_log_t::error_stream() { return error_stream_;}

std::string error_log_t::errors() const
{
	return error_stream_.str();
}

} // namespace
