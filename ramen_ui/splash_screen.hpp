// Copyright (c) 2012 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_SPLASH_SCREEN_HPP
#define RAMEN_UI_SPLASH_SCREEN_HPP

#include<ramen_ui/config.hpp>

#include<string>

#include<boost/noncopyable.hpp>
#include<boost/filesystem/path.hpp>

#include<QImage>

namespace ramen_ui
{

class splash_screen_widget_t;

class RAMEN_UI_API splash_screen_t : boost::noncopyable
{
public:

    splash_screen_t();
    ~splash_screen_t();

	int image_width() const		{ return 550;}
	int image_height() const	{ return 350;}

    void show();
    void show_message( const std::string& msg);

    void finish( QWidget *w);

private:

	bool render_splash(const boost::filesystem::path& resources_path);
	bool do_render_splash( const boost::filesystem::path& p);
	
    splash_screen_widget_t *splash_;
	QImage image_;
};

} // namespace

#endif
