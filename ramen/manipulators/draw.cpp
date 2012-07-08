// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/manipulators/draw.hpp>

#include<boost/scoped_array.hpp>

#include<OpenEXR/ImathMath.h>

#include<base/GL/gl.hpp>
#include<base/math/constants.hpp>

namespace ramen
{
namespace manipulators
{
namespace
{

void draw_arrow( const Imath::V2f& p, const Imath::V2f& q, float offset = 0)
{
	float l = ( q - p).length() / 7.0f;
		
	base::gl_begin( GL_LINES);
		base::gl_vertex2f( p.x + offset, p.y + offset);
		base::gl_vertex2f( q.x + offset, q.y + offset);
	
		Imath::V2f d( p - q);
		d.normalize();
		d *= l;
	
		float cs = Imath::Math<float>::cos( 30.0f * base::math::constants<double>::deg2rad());
		float ss = Imath::Math<float>::sin( 30.0f * base::math::constants<double>::deg2rad());
	
		base::gl_vertex2f( q.x + offset, q.y + offset);
		base::gl_vertex2f( q.x + ( d.x * cs - d.y * ss) + offset, q.y + ( d.x * ss + d.y * cs) + offset);
	
		cs = Imath::Math<float>::cos( -30.0 * base::math::constants<double>::deg2rad());
		ss = Imath::Math<float>::sin( -30.0 * base::math::constants<double>::deg2rad());
	
		base::gl_vertex2f( q.x + offset, q.y + offset);
		base::gl_vertex2f( q.x + ( d.x * cs - d.y * ss) + offset, q.y + ( d.x * ss + d.y * cs) + offset);
	base::gl_end();
}

template<class V>
void box_vertices( const Imath::Box<V>& box, float offset = 0)
{
	base::gl_vertex2f( box.min.x + offset, box.min.y + offset);
	base::gl_vertex2f( box.max.x + offset, box.min.y + offset);
	base::gl_vertex2f( box.max.x + offset, box.max.y + offset);
	base::gl_vertex2f( box.min.x + offset, box.max.y + offset);
}

} // unnamed

void draw_small_box( const Imath::V2f& p, float size)
{
	base::gl_begin( GL_LINE_LOOP);
		base::gl_vertex2f( p.x - size, p.y - size);
		base::gl_vertex2f( p.x + size, p.y - size);
		base::gl_vertex2f( p.x + size, p.y + size);
		base::gl_vertex2f( p.x - size, p.y + size);
	base::gl_end();
}

void draw_cross( const Imath::V2f& p, float xsize, float ysize, const Imath::Color3c& color, float pixel_scale)
{
	float off = shadow_offset( pixel_scale);
	
    base::gl_begin( GL_LINES);
		base::gl_color3ub( 0, 0, 0);
        base::gl_vertex2f( p.x - xsize + off, p.y + off);
        base::gl_vertex2f( p.x + xsize + off, p.y + off);
        base::gl_vertex2f( p.x + off, p.y - ysize + off);
        base::gl_vertex2f( p.x + off, p.y + ysize + off);
	
		base::gl_color3ub( color.x, color.y, color.z);
        base::gl_vertex2f( p.x - xsize, p.y);
        base::gl_vertex2f( p.x + xsize, p.y);
        base::gl_vertex2f( p.x, p.y - ysize);
        base::gl_vertex2f( p.x, p.y + ysize);
    base::gl_end();
}

void draw_ellipse( const Imath::V2f& center, float xradius, float yradius, const Imath::Color3c& color, 
				   float pixel_scale, int steps)
{
    float ang_inc = 360.0f / steps * base::math::constants<float>::deg2rad();
	float off = shadow_offset( pixel_scale);

	base::gl_color3ub( 0, 0, 0);
    base::gl_begin( GL_LINE_LOOP);
    for( float i = 0; i < steps; ++i)
		base::gl_vertex2f( center.x + off + Imath::Math<float>::cos( i * ang_inc) * xradius,
					 center.y + off + Imath::Math<float>::sin( i * ang_inc) * yradius);
	base::gl_end();

	base::gl_color3ub( color.x, color.y, color.z);
    base::gl_begin( GL_LINE_LOOP);
    for( float i = 0; i < steps; ++i)
		base::gl_vertex2f( center.x + Imath::Math<float>::cos( i * ang_inc) * xradius,
					 center.y + Imath::Math<float>::sin( i * ang_inc) * yradius);
	base::gl_end();
}

void gl_transformed_box( const Imath::Box2i& box, const Imath::M33d& m, float offset)
{
    Imath::V2d p = Imath::V2d( box.min.x, box.min.y) * m;
    base::gl_vertex2f( p.x + offset, p.y + offset);

    p = Imath::V2d( box.max.x, box.min.y) * m;
    base::gl_vertex2f( p.x + offset, p.y + offset);

    p = Imath::V2d( box.max.x, box.max.y) * m;
    base::gl_vertex2f( p.x + offset, p.y + offset);

    p = Imath::V2d( box.min.x, box.max.y) * m;
    base::gl_vertex2f( p.x + offset, p.y + offset);
}

void draw_bezier_curve( const Imath::V2f& a, const Imath::V2f& b, const Imath::V2f& c, const Imath::V2f& d, 
						const Imath::Color3c& color, float pixel_scale, int steps)
{
	float off = shadow_offset( pixel_scale);
	
	GLfloat ctrlpoints[4][3];
    ctrlpoints[0][0] = a.x + off;
    ctrlpoints[0][1] = a.y + off;
    ctrlpoints[0][2] = 0;

    ctrlpoints[1][0] = b.x + off;
    ctrlpoints[1][1] = b.y + off;
    ctrlpoints[1][2] = 0;

    ctrlpoints[2][0] = c.x + off;
    ctrlpoints[2][1] = c.y + off;
    ctrlpoints[2][2] = 0;

    ctrlpoints[3][0] = d.x + off;
    ctrlpoints[3][1] = d.y + off;
    ctrlpoints[3][2] = 0;

	// for some reason, this code generates 
	// an OpenGL error, and Ramen crashes...
	base::gl_color3ub( 0, 0, 0);
    glMap1f( GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[0][0]);

    glBegin( GL_LINE_STRIP);
    for (int i = 0; i <= steps; ++i)
		glEvalCoord1f( ( GLfloat) i/ (float) steps);
    glEnd();

    ctrlpoints[0][0] = a.x;
    ctrlpoints[0][1] = a.y;
    ctrlpoints[0][2] = 0;

    ctrlpoints[1][0] = b.x;
    ctrlpoints[1][1] = b.y;
    ctrlpoints[1][2] = 0;

    ctrlpoints[2][0] = c.x;
    ctrlpoints[2][1] = c.y;
    ctrlpoints[2][2] = 0;

    ctrlpoints[3][0] = d.x;
    ctrlpoints[3][1] = d.y;
    ctrlpoints[3][2] = 0;

	base::gl_color3ub( color.x, color.y, color.z);
    glMap1f( GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[0][0]);

    glBegin( GL_LINE_STRIP);
    for (int i = 0; i <= steps; ++i)
		glEvalCoord1f( ( GLfloat) i / (float) steps);
    glEnd();
}

// axes
void draw_xy_axes( const Imath::V2f& c, const Imath::V2f& x, const Imath::V2f& y, float xscale,
				   const Imath::Color3c& xcol, const Imath::Color3c& ycol, float pixel_scale)
{
	
	Imath::V2f xx( x.x * xscale + c.x, x.y + c.y);
	Imath::V2f yy( y.x * xscale + c.x, y.y + c.y);
	float offset = shadow_offset( pixel_scale);

	base::gl_color3ub( 0, 0, 0);
    draw_arrow( c, xx, offset);
    draw_arrow( c, yy, offset);
	
    base::gl_color3ub( xcol.x, xcol.y, xcol.z);
    draw_arrow( c, xx);

    base::gl_color3ub( ycol.x, ycol.y, ycol.z);
    draw_arrow( c, yy);
}

void draw_xy_axes( const Imath::V2f& c, float xsize, float ysize, float angle, float xscale,
				   const Imath::Color3c& xcol, const Imath::Color3c& ycol, float pixel_scale)
{
    float cs = Imath::Math<float>::cos( (double) angle * base::math::constants<double>::deg2rad());
    float ss = Imath::Math<float>::sin( (double) angle * base::math::constants<double>::deg2rad());
	draw_xy_axes( c, Imath::V2f( cs, ss) * xsize, Imath::V2f( ss, -cs) * ysize, xscale,
					xcol, ycol, pixel_scale);
}

// util
float shadow_offset( float pixel_scale) { return 2.0f / pixel_scale;}

void draw_box( const Imath::Box2i& box, const Imath::Color3c& color, float pixel_scale, bool draw_corners)
{
	Imath::Box2f box2( Imath::V2f( box.min), Imath::V2f( box.max));
	draw_box( box2, color, pixel_scale, draw_corners);
}

void draw_box( const Imath::Box2f& box, const Imath::Color3c& color, float pixel_scale, bool draw_corners)
{
	float off = shadow_offset( pixel_scale);

	// shadow
	base::gl_color3ub( 0, 0, 0);
	base::gl_begin( GL_LINE_LOOP);
		box_vertices( box, off);
	base::gl_end();

	if( draw_corners)
	{
		base::gl_begin( GL_POINTS);
			box_vertices( box, off);
		base::gl_end();
	}
	
	// main
    base::gl_color3ub( color.x, color.y, color.z);
	base::gl_begin( GL_LINE_LOOP);
		box_vertices( box);
	base::gl_end();
	
	if( draw_corners)
	{
		base::gl_begin( GL_POINTS);
			box_vertices( box);
		base::gl_end();
	}	
}

} // namespace
} // namespace
