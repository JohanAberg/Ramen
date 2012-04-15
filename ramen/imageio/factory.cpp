// Copyright (c) 2010 Esteban Tovagliari

#include<ramen/imageio/factory.hpp>

#include<algorithm>

#include<boost/foreach.hpp>
#include<boost/filesystem/fstream.hpp>

#include<ramen/imageio/algorithm.hpp>
#include<ramen/imageio/exceptions.hpp>

namespace ramen
{
namespace imageio
{

factory_impl::factory_impl() : detect_size_(0), detect_buffer_(0) {}
factory_impl::~factory_impl() { delete detect_buffer_;}

void factory_impl::init() {}

bool factory_impl::register_image_format( std::auto_ptr<format_t> format)
{
    detect_size_ = std::max( detect_size_, format->detect_size());
    format->add_extensions( extensions_);
    formats_.push_back( format);
    return true;
}

bool factory_impl::is_image_file( const boost::filesystem::path& p) const
{
    const_iterator it = format_for_extension( p);

    if( it != formats_.end())
		return true;

    it = format_for_file_contents( p);

    if( it != formats_.end())
		return true;

	return false;
}

bool factory_impl::is_image_format_tag( const std::string& tag) const
{
	return format_for_tag( tag) != formats_.end();
}

std::auto_ptr<reader_t> factory_impl::reader_for_image( const boost::filesystem::path& p) const
{
    const_iterator it = format_for_extension( p);

    if( it != formats_.end())
		return it->reader( p);

    it = format_for_file_contents( p);

    if( it != formats_.end())
		return it->reader( p);

	throw unknown_image_format();
    return std::auto_ptr<reader_t>();
}

std::auto_ptr<writer_t> factory_impl::writer_for_tag( const std::string& tag) const
{
    const_iterator it = format_for_tag( tag);

    if( it != formats_.end())
    {
        std::auto_ptr<writer_t> w( it->writer());
        return w;
    }

	throw unknown_image_format();
	return std::auto_ptr<writer_t>();
}

factory_impl::const_iterator factory_impl::format_for_extension( const boost::filesystem::path& p) const
{
    std::string ext( p.extension().string());

    if( !ext.empty())
    {
        for( const_iterator it( formats_.begin()); it != formats_.end(); ++it)
        {
            if( it->input() &&  it->check_extension( ext))
            return it;
        }
    }

    return formats_.end();
}

factory_impl::const_iterator factory_impl::format_for_file_contents( const boost::filesystem::path& p) const
{
    boost::filesystem::ifstream ifile( p, std::ios::in | std::ios::binary);

    if( ifile.is_open() && ifile.good())
	{
	    if( !detect_buffer_)
	        detect_buffer_ = new char[detect_size_];

	    ifile.read( detect_buffer_, detect_size_);
	
	    for( const_iterator it( formats_.begin()); it != formats_.end(); ++it)
	    {
	        if( it->input() && it->detect( detect_buffer_))
	            return it;
	    }
	}

    return formats_.end();
}

factory_impl::const_iterator factory_impl::format_for_tag( const std::string& tag) const
{
    for( const_iterator it( formats_.begin()); it != formats_.end(); ++it)
    {
        if( it->output() && ( it->tag() == tag))
            return it;
    }

    return formats_.end();
}

} // namespace
} // namespace
