// Copyright (c) 2010 Esteban Tovagliari

#include<ramen/params/composite_param.hpp>

#include<string>
#include<stdexcept>

#include<boost/bind.hpp>
#include<boost/foreach.hpp>

#include<adobe/algorithm/for_each.hpp>

#include<QWidget>
#include<QVBoxLayout>
#include<QFrame>

#include<ramen/params/param_set.hpp>

#include<ramen/anim/track.hpp>
	
#include<ramen/ui/inspector/inspector.hpp>

namespace ramen
{

composite_param_t::composite_param_t() : param_t(), create_track_( true) {}
composite_param_t::composite_param_t( const std::string& name) : param_t( name), create_track_( true) {}
composite_param_t::composite_param_t( const composite_param_t& other) : param_t( other), params_( other.params_)
{
	create_track_ = other.create_track_;
}

void composite_param_t::init()
{
    adobe::for_each( params(), boost::bind( &param_t::init, _1));
}

void composite_param_t::set_param_set( param_set_t *parent)
{
    param_t::set_param_set( parent);
    adobe::for_each( params(), boost::bind( &param_t::set_param_set, _1, parent));
}

void composite_param_t::do_add_param( param_t *p)
{
    p->set_param_set( param_set());

    if( param_set())
    {
        try
        {
            param_set()->find( p->id());
            throw std::runtime_error( std::string( "Duplicated param id found: ").append( p->id()));
        }
        catch( std::runtime_error& e) {}
    }
    else
    {
        if( find( p->id()))
            throw std::runtime_error( std::string( "Duplicated param id found: ").append( p->id()));
    }

    params().push_back( p);
}

const param_t *composite_param_t::find( const std::string& id) const
{
    composite_param_t& self = const_cast<composite_param_t&>( *this);
    return self.find( id);
}

param_t *composite_param_t::find( const std::string& id)
{
    BOOST_FOREACH( param_t& p, params())
    {
        if( p.id() == id)
            return &p;

        if( composite_param_t *cp = dynamic_cast<composite_param_t*>( &p))
        {
            param_t *q = cp->find( id);

            if( q)
                return q;
        }
    }

    return 0;
}

void composite_param_t::set_frame( float frame)
{
	emit_param_changed( time_changed);
    adobe::for_each( params(), boost::bind( &param_t::set_frame, _1, frame));	
}

void composite_param_t::do_create_tracks( anim::track_t *parent)
{
	if( create_track_)
	{
	    std::auto_ptr<anim::track_t> t( new anim::track_t( name()));
		adobe::for_each( params(), boost::bind( &param_t::create_tracks, _1, t.get()));

	    if( t->num_children() != 0)
			parent->add_child( t);
	}
	else
		adobe::for_each( params(), boost::bind( &param_t::create_tracks, _1, parent));
}

void composite_param_t::do_evaluate( float frame)
{
    adobe::for_each( params(), boost::bind( &param_t::evaluate, _1, frame));
}

void composite_param_t::do_add_to_hash( util::hash_generator_t& hash_gen) const
{
    BOOST_FOREACH( const param_t& p, params())
		p.add_to_hash( hash_gen);
}

void composite_param_t::do_update_widgets()
{
    adobe::for_each( params_, boost::bind( &param_t::update_widgets, _1));
}

void composite_param_t::do_enable_widgets( bool e)
{
    adobe::for_each( params_, boost::bind( &param_t::enable_widgets, _1, e));
}

void composite_param_t::do_format_changed( const Imath::Box2i& new_format, float aspect, const Imath::V2f& proxy_scale)
{
    adobe::for_each( params_, boost::bind( &param_t::format_changed, _1, new_format, aspect, proxy_scale));
}

void composite_param_t::convert_relative_paths( const boost::filesystem::path& old_base, const boost::filesystem::path& new_base)
{
    adobe::for_each( params_, boost::bind( &param_t::convert_relative_paths, _1, old_base, new_base));
}

void composite_param_t::make_paths_absolute()
{
    adobe::for_each( params_, boost::bind( &param_t::make_paths_absolute, _1));
}

void composite_param_t::make_paths_relative()
{
    adobe::for_each( params_, boost::bind( &param_t::make_paths_relative, _1));
}

// util
void composite_param_t::apply_function( const boost::function<void ( param_t*)>& f)
{ 
	param_t::apply_function( f);
	
    BOOST_FOREACH( param_t& p, params())
		p.apply_function( f);
}

void composite_param_t::write( serialization::yaml_oarchive_t& out) const
{
	param_t::write( out);
    adobe::for_each( params_, boost::bind( &param_t::write, _1, boost::ref( out)));
}

QWidget *composite_param_t::do_create_widgets()
{
    QWidget *widget = new QWidget();
    create_widgets_inside_widget( widget);
    return widget;
}

void composite_param_t::create_widgets_inside_widget( QWidget *parent)
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins( 0, 0, 0, 0);
    layout->setSpacing( 5);
    layout->setSizeConstraint( QLayout::SetFixedSize);

    BOOST_FOREACH( param_t& p, params())
    {
        if( !p.secret())
        {
            QWidget *w = p.create_widgets();

            if( w)
                layout->addWidget( w);
        }
    }

    layout->addStretch();
    parent->setLayout( layout);
}

} // namespace
