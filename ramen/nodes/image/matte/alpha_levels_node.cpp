// Copyright (c) 2010 Esteban Tovagliari

#include<ramen/nodes/image/matte/alpha_levels_node.hpp>

#include<ramen/gil/extension/algorithm/tbb/tbb.hpp>

#include<ramen/params/float_param.hpp>
#include<ramen/params/bool_param.hpp>

namespace ramen
{
namespace image
{
namespace
{

struct remap_alpha_premult_fun
{
    remap_alpha_premult_fun( float lo, float hi) : alpha_low_( lo), alpha_high_( hi) {}

    image::pixel_t operator()( const image::pixel_t& p) const
    {
		float r = boost::gil::get_color( p, boost::gil::red_t());
		float g = boost::gil::get_color( p, boost::gil::green_t());
		float b = boost::gil::get_color( p, boost::gil::blue_t());
		float a = boost::gil::get_color( p, boost::gil::alpha_t());
	
		if( a != 0.0f)
		{
			r /= a;
			g /= a;
			b /= a;
		}
		else
		{
			r = g = b = 0.0f;
		}
	
		if( a <= alpha_low_)
			a = 0;
		else
		{
			if( a >= alpha_high_ || alpha_high_ == alpha_low_)
				a = 1;
			else
				a = ( a - alpha_low_) / ( alpha_high_ - alpha_low_);
		}
	
		return image::pixel_t( r * a, g * a, b * a, a);
    }

private:

    float alpha_low_, alpha_high_;
};

struct remap_alpha_unpremult_fun
{
    remap_alpha_unpremult_fun( float lo, float hi) : alpha_low_( lo), alpha_high_( hi) {}

    image::pixel_t operator()( const image::pixel_t& p) const
    {
		float r = boost::gil::get_color( p, boost::gil::red_t());
		float g = boost::gil::get_color( p, boost::gil::green_t());
		float b = boost::gil::get_color( p, boost::gil::blue_t());
		float a = boost::gil::get_color( p, boost::gil::alpha_t());
	
		if( a <= alpha_low_)
			a = 0;
		else
		{
			if( a >= alpha_high_ || alpha_high_ == alpha_low_)
				a = 1;
			else
				a = ( a - alpha_low_) / ( alpha_high_ - alpha_low_);
		}

		return image::pixel_t( r, g, b, a);
    }

private:

    float alpha_low_, alpha_high_;
};

}

alpha_levels_node_t::alpha_levels_node_t() : pointop_node_t() { set_name("alpha_levels");}

void alpha_levels_node_t::do_create_params()
{
    std::auto_ptr<float_param_t> q( new float_param_t( "Alpha Low"));
    q->set_id( "alpha_low");
    q->set_default_value( 0);
    q->set_range( 0, 1);
    q->set_step( 0.05);
    add_param( q);

    q.reset( new float_param_t( "Alpha High"));
    q->set_id( "alpha_high");
    q->set_default_value( 1);
    q->set_range( 0, 1);
    q->set_step( 0.05);
    add_param( q);

    std::auto_ptr<bool_param_t> p( new bool_param_t( "Premultiplied"));
    p->set_id( "premult");
    p->set_default_value( true);
    add_param( p);
}

void alpha_levels_node_t::do_process( const image::const_image_view_t& src, const image::image_view_t& dst, const render::context_t& context)
{
    if( get_value<bool>( param( "premult")))
    {
		boost::gil::tbb_transform_pixels( src, dst, remap_alpha_premult_fun( get_value<float>( param( "alpha_low")),
										  get_value<float>( param( "alpha_high"))));
    }
    else
    {
		boost::gil::tbb_transform_pixels( src, dst, remap_alpha_unpremult_fun( get_value<float>( param( "alpha_low")),
										    get_value<float>( param( "alpha_high"))));
    }
}

// factory
node_t *create_alpha_levels_node() { return new alpha_levels_node_t();}

const node_metaclass_t *alpha_levels_node_t::metaclass() const { return &alpha_levels_node_metaclass();}

const node_metaclass_t& alpha_levels_node_t::alpha_levels_node_metaclass()
{
    static bool inited( false);
    static node_metaclass_t info;

    if( !inited)
    {
        info.id = "image.builtin.alpha_levels";
        info.major_version = 1;
        info.minor_version = 0;
        info.menu = "Image";
        info.submenu = "Matte";
        info.menu_item = "Alpha Levels";
        info.create = &create_alpha_levels_node;
        inited = true;
    }

    return info;
}

static bool registered = node_factory_t::instance().register_node( alpha_levels_node_t::alpha_levels_node_metaclass());

} // namespace
} // namespace
