// Copyright (c) 2010 Esteban Tovagliari

#include<ramen/ui/palette.hpp>

#include<QApplication>
#include<QPalette>

#include<ramen/assert.hpp>

#include<iostream>

namespace ramen
{
namespace ui
{

struct palette_impl::impl
{
	impl() { rebuild_palette();}

	void rebuild_palette()
	{
		QColor bgcol = QApplication::palette().color( QPalette::Base);
		QColor fgcol = QApplication::palette().color( QPalette::Text);

		error_color_ = Imath::Color3c( 255, 0, 255);

	    colors_["background"]   = Imath::Color3c( bgcol.red(), bgcol.green(), bgcol.blue());
	    colors_["text"]   = Imath::Color3c( fgcol.red(), fgcol.green(), fgcol.blue());

	    colors_["front plug"]   = Imath::Color3c( 255, 0, 0);
	    colors_["back plug"]    = Imath::Color3c( 0, 255, 0);
	    colors_["matte plug"]   = Imath::Color3c( 0, 0, 255);
	    colors_["out plug"]	    = Imath::Color3c( 255, 255, 0);
		colors_["optional plug"] = Imath::Color3c( 0, 255, 0);

		colors_["node_shadow"]		= Imath::Color3c( 64, 64, 64);
		colors_["node_bg"]			= Imath::Color3c( 150, 150, 150);
		colors_["node_picked_bg"]	= Imath::Color3c( 200, 140, 0);
		colors_["node_error_bg"]	= Imath::Color3c( 150, 25, 0);

	    colors_["node_active_out"]  = Imath::Color3c( 255, 0, 0);
	    colors_["node_context_out"] = Imath::Color3c( 0, 0, 255);

		// manipulators
		colors_["manipulator"]			= Imath::Color3c( 255, 255, 0);
		colors_["active_manipulator"]	= Imath::Color3c( 255, 0, 0);

		colors_["x_axis"] = Imath::Color3c( 255, 0, 0);
		colors_["y_axis"] = Imath::Color3c( 0, 255, 0);
		colors_["z_axis"] = Imath::Color3c( 0, 0, 255);

		// interface elems
		colors_["grid"] = Imath::Color3c( 132, 132, 132);
		colors_["box_pick"] = Imath::Color3c( 255, 0, 0);
	}

	const Imath::Color3c& get_color( const std::string& k) const
	{
		std::map<std::string, Imath::Color3c>::const_iterator it;
	    it = colors_.find( k);

		if( it != colors_.end())
			return it->second;

		#ifndef NDEBUG
			std::cout << "Palette: color not found " << k << "\n";
		#endif

		return error_color_;
	}

    std::map<std::string, Imath::Color3c> colors_;
	Imath::Color3c error_color_;
};

palette_impl::palette_impl() { pimpl_.reset( new impl());}

palette_impl::~palette_impl()
{
	// do not remove. It's needed by auto_ptr.
}

void palette_impl::rebuild() { pimpl_->rebuild_palette();}

const Imath::Color3c& palette_impl::color( const std::string& k) const
{
	return pimpl_->get_color( k);
}

QColor palette_impl::qcolor( const std::string& k) const
{
	Imath::Color3c c = pimpl_->get_color( k);
	return QColor( c.x, c.y, c.z, 255);
}

bool palette_impl::find_color( const std::string& k) const
{
	// TODO: implement this.
	RAMEN_ASSERT( 0);
}

void palette_impl::set_color( const std::string& k, const Imath::Color3c& c)
{
	// TODO: implement this.
	RAMEN_ASSERT( 0);
}

void palette_impl::set_color( const std::string& k, const QColor& c)
{
	// TODO: implement this.
	RAMEN_ASSERT( 0);
}

} // namespace
} // namespace
