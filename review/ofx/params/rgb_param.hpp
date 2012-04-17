// Copyright (c) 2010 Esteban Tovagliari

#ifndef RAMEN_OFX_RGB_PARAM_HPP
#define RAMEN_OFX_EGB_PARAM_HPP

#include"ofxCore.h"
#include"ofxParam.h"

#include"ofxhPropertySuite.h"
#include"ofxhParam.h"

namespace ramen
{

namespace ofx
{

class image_effect_t;

class rgb_color_param_t : public OFX::Host::Param::RGBInstance
{
public:

    rgb_color_param_t( const std::string& name, OFX::Host::Param::Descriptor& descriptor, OFX::Host::Param::SetInstance* instance = 0);

    OfxStatus copy( const Instance &instance, OfxTime offset);
    OfxStatus copy( const Instance &instance, OfxTime offset, OfxRangeD range);

    OfxStatus get( double& r, double& g, double& b);
    OfxStatus get( OfxTime time, double& r, double& g, double& b);
    OfxStatus set( double r, double g, double b);
    OfxStatus set( OfxTime time, double r, double g, double b);

    OfxStatus getNumKeys( unsigned int& nKeys) const;
    OfxStatus getKeyTime( int nth, OfxTime& time) const;
    OfxStatus getKeyIndex( OfxTime time, int direction, int& index) const;
    OfxStatus deleteKey( OfxTime time);
    OfxStatus deleteAllKeys();

	// ui
	virtual void setEnabled();
	
private:

    image_effect_t *effect_;
};

} // namespace
} // namespace

#endif
