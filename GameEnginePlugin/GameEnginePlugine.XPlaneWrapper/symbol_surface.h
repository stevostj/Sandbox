#ifndef GAMEENGINEPLUGINXPLANEWRAPPER_SYMBOLSURFACE_H_
#define GAMEENGINEPLUGINXPLANEWRAPPER_SYMBOLSURFACE_H_

#include <gl/GL.h>
#include <list>
#include <vector>

#include "cigi4types.h"

namespace gep_xpw {

	/// <summary>
	/// Reprents 2D surface that manages and renders primitive graphical symbols.
	/// </summary>
	class SymbolSurface {
	public:

		SymbolSurface();

		int Initialize(XPLMDisplayApi* display_api_hooks, XPLMGraphicsApi* graphics_api_hooks);

		void Update(CIGI_SYMBOL_SURFACE_DEF const & surface);

		void Update(std::list<CIGI_SYMBOL_POLYGON_DEF> const & polygons);

		void Render();

	private:
		void RenderPolygons();

		void ConvertToGLArray(CIGI_SYMBOL_POLYGON_DEF const& polygon, std::vector<GLfloat>& gl_vector, unsigned int num_vertices);

		CIGI_SYMBOL_SURFACE_DEF surface_;

		std::list<CIGI_SYMBOL_POLYGON_DEF> polygons_;

		XPLMDisplayApi* display_api_hooks_; 
		XPLMGraphicsApi* graphics_api_hooks_;

		int screen_width_;
		int screen_height_;

		float x_min_;
		float u2x_scale_;
		float y_min_;
		float v2y_scale_;

	};
}


#endif