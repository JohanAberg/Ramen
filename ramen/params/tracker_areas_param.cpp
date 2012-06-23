// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/params/tracker_areas_param.hpp>

#include<ramen/app/application.hpp>

#include<ramen/assert.hpp>

#include<ramen/params/param_set.hpp>

namespace ramen
{
namespace params
{

float tracker_areas_t::min_reference_radius()	{ return 3;}
float tracker_areas_t::min_search_radius()		{ return 5;}

tracker_areas_t::tracker_areas_t( int r, int s)
{
    set_areas( Imath::Box2f( Imath::V2f( r, r), Imath::V2f( r, r)),
                Imath::Box2f( Imath::V2f( s, s), Imath::V2f( s, s)));
}

tracker_areas_t::tracker_areas_t( const Imath::Box2f& ref, const Imath::Box2f& srch) { set_areas( ref, srch);}

void tracker_areas_t::set_areas( const Imath::Box2f& r, const Imath::Box2f& s)
{
    reference.min.x = std::max( r.min.x, min_reference_radius());
    reference.min.y = std::max( r.min.y, min_reference_radius());
    reference.max.x = std::max( r.max.x, min_reference_radius());
    reference.max.y = std::max( r.max.y, min_reference_radius());

    search.min.x = std::max( std::max( s.min.x, reference.min.x + 2), min_search_radius());
    search.min.y = std::max( std::max( s.min.y, reference.min.y + 2), min_search_radius());
    search.max.x = std::max( std::max( s.max.x, reference.max.x + 2), min_search_radius());
    search.max.y = std::max( std::max( s.max.y, reference.max.y + 2), min_search_radius());
}

void tracker_areas_t::set_reference_area( const Imath::Box2f& r)	{ set_areas( r, search);}
void tracker_areas_t::set_search_area( const Imath::Box2f& s)		{ set_areas( reference, s);}

bool tracker_areas_t::operator==( const tracker_areas_t& other) const
{
    return reference == other.reference && search == other.search;
}

bool tracker_areas_t::operator!=( const tracker_areas_t& other) const
{
    return reference != other.reference || search != other.search;
}

void operator>>( const YAML::Node& in, tracker_areas_t& areas)
{
    in[0] >> areas.reference.min.x;
    in[1] >> areas.reference.min.y;
    in[2] >> areas.reference.max.x;
    in[3] >> areas.reference.max.y;
    in[4] >> areas.search.min.x;
    in[5] >> areas.search.min.y;
    in[6] >> areas.search.max.x;
    in[7] >> areas.search.max.y;
}

YAML::Emitter& operator<<( YAML::Emitter& out, const tracker_areas_t& areas)
{
    out << YAML::Flow << YAML::BeginSeq
        << ( int) areas.reference.min.x
        << ( int) areas.reference.min.y
        << ( int) areas.reference.max.x
        << ( int) areas.reference.max.y
        << ( int) areas.search.min.x
        << ( int) areas.search.min.y
        << ( int) areas.search.max.x
        << ( int) areas.search.max.y
        << YAML::EndSeq;
    return out;
}

tracker_areas_param_t::tracker_areas_param_t() : static_param_t( "Tracker Areas")
{
    set_track_mouse( false);
    value().assign( tracker_areas_t( 15, 30));

    ref_minx_ = 0;
    ref_miny_ = 0;
    ref_maxx_ = 0;
    ref_maxy_ = 0;
    search_minx_ = 0;
    search_miny_ = 0;
    search_maxx_ = 0;
    search_maxy_ = 0;
}

tracker_areas_param_t::tracker_areas_param_t( const tracker_areas_param_t& other) : static_param_t( other)
{
    ref_minx_ = 0;
    ref_miny_ = 0;
    ref_maxx_ = 0;
    ref_maxy_ = 0;
    search_minx_ = 0;
    search_miny_ = 0;
    search_maxx_ = 0;
    search_maxy_ = 0;
}

void tracker_areas_param_t::set_value( const tracker_areas_t& areas)
{
    if( can_undo())
        param_set()->add_command( this);

    value().assign( areas);
    emit_param_changed( user_edited);
}

void tracker_areas_param_t::do_read( const serialization::yaml_node_t& node)
{
    tracker_areas_t val;
    if( node.get_optional_value( "value", val))
        value().assign( val);
}

void tracker_areas_param_t::do_write( serialization::yaml_oarchive_t& out) const
{
    tracker_areas_t val( get_value<tracker_areas_t>( *this));
    out << YAML::Key << "value" << YAML::Value << val;
}

} // namespace
} // namespace
