// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/movieio/factory.hpp>

#include<algorithm>

#include<boost/foreach.hpp>
#include<boost/filesystem/fstream.hpp>

#include<ramen/imageio/factory.hpp>

#include<ramen/movieio/image_seq_reader.hpp>
#include<ramen/movieio/image_seq_writer.hpp>

namespace ramen
{
namespace movieio
{

factory_impl::factory_impl() : detect_size_(0), detect_buffer_(0)
{
	imageio_extensions_added_ = false;
}

factory_impl::~factory_impl() { delete detect_buffer_;}

void factory_impl::init()
{
    imageio::factory_t::Instance().init();
}

bool factory_impl::register_movie_format( std::auto_ptr<format_t> format)
{
    detect_size_ = std::max( detect_size_, format->detect_size());
    format->add_extensions( extensions_);
    formats_.push_back( format);
    return true;
}

const std::vector<std::string>& factory_impl::extensions() const
{
	if( !imageio_extensions_added_)
	{
	    BOOST_FOREACH( const std::string& ext, imageio::factory_t::Instance().extensions())
			extensions_.push_back( ext);
		
		imageio_extensions_added_ = true;
	}
	
	return extensions_;
}

std::auto_ptr<reader_t> factory_impl::create_reader( const boost::filesystem::path& p, bool sequence) const
{
	RAMEN_ASSERT( p.is_absolute());
	
	if( imageio::factory_t::Instance().is_image_file( p))
	{
		std::auto_ptr<reader_t> result( new image_seq_reader_t( p, sequence));
		return result;
	}

    const_iterator it = format_for_extension( p);

    if( it != formats_.end())
		return it->reader( p);

    it = format_for_file_contents( p);

    if( it != formats_.end())
		return it->reader( p);
	
	throw unknown_movie_format();
	return std::auto_ptr<reader_t>();
}

std::auto_ptr<reader_t> factory_impl::create_reader( const filesystem::path_sequence_t& seq) const
{
	// TODO: we need to do something better when we add movie formats.
	std::auto_ptr<reader_t> result( new image_seq_reader_t( seq));
	return result;
}

std::auto_ptr<writer_t> factory_impl::writer_for_tag( const std::string& tag) const
{
	if( imageio::factory_t::Instance().is_image_format_tag( tag))
	{
	}

    const_iterator it = format_for_tag( tag);

    if( it != formats_.end())
		return it->writer();

	throw unknown_movie_format();
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
	if( detect_size_ == 0)
		return formats_.end();
	
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
