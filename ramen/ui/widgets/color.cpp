// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/widgets/color.hpp>

#include<cmath>

#include<algorithm>

#include<OpenEXR/ImathFun.h>

#include<ramen/assert.hpp>

namespace ramen
{
namespace ui
{

color_t::color_t() {}

color_t::color_t( double r, double g, double b, double a)
{
    red_ = r;
    green_ = g;
    blue_ = b;
    alpha_ = a;
}

color_t::operator QColor()
{
    return QColor( Imath::clamp( red_	, 0.0, 1.0) * 255,
           Imath::clamp( green_	, 0.0, 1.0) * 255,
           Imath::clamp( blue_	, 0.0, 1.0) * 255,
           Imath::clamp( alpha_	, 0.0, 1.0) * 255);
}

color_t& color_t::operator*=( double s)
{
    red_    *= s;
    green_  *= s;
    blue_   *= s;
    return *this;
}

void color_t::apply_gamma( double g)
{
    RAMEN_ASSERT( g > 0.0 && "color_t::applyGamma: gamma must be > 0");
    red_    = std::pow( std::max( red_  , 0.0), g);
    green_  = std::pow( std::max( green_, 0.0), g);
    blue_   = std::pow( std::max( blue_ , 0.0), g);
}

void color_t::clamp()
{
    red_    = std::max( std::min( red_, 1.0), 0.0);
    green_  = std::max( std::min( green_, 1.0), 0.0);
    blue_   = std::max( std::min( blue_, 1.0), 0.0);
}

void color_t::to_hsv(double& h, double& s, double& v) const
{
    double r = red_;
    double g = green_;
    double b = blue_;

    // normalize RGB to 0 .. 1
    if( r < 0) r = 0;
    if( g < 0) g = 0;
    if( b < 0) b = 0;

    double scale = std::max( r, std::max( g, b));

    if( scale < 1.0)
    scale = 1.0;

    r /= scale;
    g /= scale;
    b /= scale;

    double maxv = std::max( r, std::max( g, b));
    double minv = std::min( r, std::min( g, b));

    h = 0;
    s = 0;
    v = maxv * scale;

    if( maxv != minv)
    {
        const double f = ( r == minv) ? ( g - b) : (( g == minv) ? ( b - r) : ( r - g)),
                i = ( r == minv) ? 3.0 : (( g == minv) ? 5.0 : 1.0);

        h = ( i - f / ( maxv - minv));

        if( h >= 6.0)
            h -= 6.0;

        h /= 6.0;
        s = ( maxv - minv) / maxv;
    }
}

color_t color_t::from_hsv( double h, double s, double v)
{
    double scale = std::max( v, 1.0);
    v /= scale;

    double r = 0, g = 0, b = 0;

    if( h == 0 && s == 0)
    r = g = b = v;
    else
    {
        h *= 6.0;
        const int i = (int) std::floor( h);
        const double f = ( i & 1) ? ( h - i) : ( 1.0 - h + i),
                 m = v * ( 1.0 - s),
                 n = v * ( 1.0 - s * f);
        switch(i)
        {
            case 6:
            case 0: r = v; g = n; b = m; break;
            case 1: r = n; g = v; b = m; break;
            case 2: r = m; g = v; b = n; break;
            case 3: r = m; g = n; b = v; break;
            case 4: r = n; g = m; b = v; break;
            case 5: r = v; g = m; b = n; break;
        }
    }

    r *= scale;
    g *= scale;
    b *= scale;

    return color_t( r, g, b);
}

} // ui
} // ramen
