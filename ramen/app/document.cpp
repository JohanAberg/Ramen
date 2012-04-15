// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/app/document.hpp>

#include<boost/bind.hpp>
#include<boost/filesystem/fstream.hpp>

#include<adobe/algorithm/for_each.hpp>

#include<ramen/app/application.hpp>

#include<ramen/serialization/yaml_oarchive.hpp>

namespace ramen
{

document_impl::document_impl() : dirty_( false) {}

void document_impl::set_file( const boost::filesystem::path& p)
{
	RAMEN_ASSERT( p.empty() || p.is_absolute());

    file_ = p;
	composition().set_composition_dir( file_.parent_path());
}

void document_impl::load( serialization::yaml_iarchive_t& in)
{
	RAMEN_ASSERT( has_file());
	composition().read( in);
}

void document_impl::save( serialization::yaml_oarchive_t& out) const
{
	RAMEN_ASSERT( out.header_written());
	RAMEN_ASSERT( out.map_level() == 1);

	composition().write( out);
}

} // namespace
