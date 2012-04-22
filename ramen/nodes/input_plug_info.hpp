// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_INPUT_PLUG_INFO_HPP
#define	RAMEN_INPUT_PLUG_INFO_HPP

#include<ramen/ui/palette.hpp>

#include<boost/flyweight.hpp>

namespace ramen
{

class input_plug_info_t
{
public:

    input_plug_info_t()
    {
        color_ = ui::palette_t::instance().color( "front plug");
    }

    input_plug_info_t( const Imath::Color3c& color) : color_(color), tooltip_( "Input") {}
    input_plug_info_t( const Imath::Color3c& color, const std::string& tooltip) : color_( color), tooltip_( tooltip) {}

    const Imath::Color3c& color() const         { return color_;}
    void set_color( const Imath::Color3c& c)	{ color_ = c;}

    const std::string& tooltip() const	    { return tooltip_;}
    void set_tooltip( const std::string& s) { tooltip_ = s;}

private:

    Imath::Color3c color_;
    boost::flyweight<std::string> tooltip_;
};

} // namespace

#endif
