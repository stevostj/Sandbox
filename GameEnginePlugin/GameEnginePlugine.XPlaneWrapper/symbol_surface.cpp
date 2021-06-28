#include <windows.h> 
#include <GL/gl.h> 
#include <GL/glu.h> 
#include <vector>

#include "set_xplm_api_hooks.h"
#include "helper_methods.h"

#include "symbol_surface.h"

gep_xpw::SymbolSurface::SymbolSurface() : 
	display_api_hooks_(nullptr), graphics_api_hooks_(nullptr), 
	surface_({}), polygons_(std::list<CIGI_SYMBOL_POLYGON_DEF>()), 
	screen_width_(0), screen_height_(0), 
	x_min_(0.0f), u2x_scale_(1.0f), y_min_(0.0f), v2y_scale_(1.0f)
{
}

void gep_xpw::SymbolSurface::Render()
{
	graphics_api_hooks_->SetGraphicsState(
		0,        // No fog, equivalent to glDisable(GL_FOG);
		1,        // One texture, equivalent to glEnable(GL_TEXTURE_2D);
		0,        // No lighting, equivalent to glDisable(GL_LIGHT0);
		0,        // No alpha testing, e.g glDisable(GL_ALPHA_TEST);
		1,        // Use alpha blending, e.g. glEnable(GL_BLEND);
		0,        // No depth read, e.g. glDisable(GL_DEPTH_TEST);
		0);        // No depth write, e.g. glDepthMask(GL_FALSE);

	// FUTURE: Remove conditional when opengl calls (or proxy calls) are supported in a unit test context
	if(graphics_api_hooks_->SetGraphicsState == XPLMSetGraphicsState)
	   RenderPolygons();

}

void gep_xpw::SymbolSurface::ConvertToGLArray(CIGI_SYMBOL_POLYGON_DEF const& polygon, std::vector<GLfloat> & gl_vector, unsigned int num_vertices)
{
	for (unsigned int i = 0; i < num_vertices; ++i) 
	{
		gl_vector.push_back(polygon.line[i].vertex_u); // IN PROGRESS: transform as specified in surface definition
		gl_vector.push_back(polygon.line[i].vertex_v);
		gl_vector.push_back(0);
	}
}

/// <summary>
/// Render the polygons via OpenGL functions
/// </summary>
void gep_xpw::SymbolSurface::RenderPolygons()
{
	std::vector<GLfloat> line_vertices;
	for (std::list<CIGI_SYMBOL_POLYGON_DEF>::iterator i = polygons_.begin();
		i != polygons_.end(); i++) 
	{
		unsigned int num_vertices = (i->packet_size - CIGI_SYMBOL_POLYGON_DEF_BASE_SIZE) / CIGI_SYMBOL_VERTEX_SIZE;
		ConvertToGLArray(*i, line_vertices, num_vertices);
	}

	GLfloat * line_vertices_raw = line_vertices.data();

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, line_vertices_raw);
	glDrawArrays(GL_LINES, 0, line_vertices.size());
	glDisableClientState(GL_VERTEX_ARRAY);

}

int gep_xpw::SymbolSurface::Initialize(XPLMDisplayApi* display_api_hooks, XPLMGraphicsApi* graphics_api_hooks)
{
	int rv = 0;
	if (display_api_hooks != nullptr && graphics_api_hooks != nullptr)
	{
		display_api_hooks_ = display_api_hooks;
		graphics_api_hooks_ = graphics_api_hooks;
		display_api_hooks_->GetScreenSize(&screen_height_, &screen_width_);

		rv = (screen_height_ > 0 && screen_width_ > 0) ? 1 : 0;
	}

	return rv;
}

/// <summary>
/// Update the dimensions/scaling of the surface that symbols will be drawn on.
/// </summary>
void gep_xpw::SymbolSurface::Update(CIGI_SYMBOL_SURFACE_DEF const& surface)
{
	surface_ = surface; // a complete override is fine: FUTURE: make this thread-safe to prevent race conditions with Render.

	// IN PROGRESS: get this transformation right...
    x_min_ = screen_width_ * surface_.x_left;
	u2x_scale_ = (surface_.max_u - surface_.min_u) / (screen_width_ * surface_.y_right - x_min_);
	y_min_ = screen_height_ * surface_.z_top;
	v2y_scale_ = (surface_.max_v - surface_.min_v) / (screen_height_ * surface_.yaw_bottom - y_min_);
}

/// <summary>
/// Update the currently stored collection of polygons with updates.
/// In CIGI, symbol updates are intended to happen as infrequently as possible. 
/// Therefore, a symbol will persist until it is destroyed, and it will maintain the exact same state
//  until an update to the symbol has occured. 
/// </summary>
void gep_xpw::SymbolSurface::Update(std::list<CIGI_SYMBOL_POLYGON_DEF> const& polygons)
{
	// FUTURE: add the logic/data structures necessary to ensure unique symbols, parent-child relationships, 
	// correlation to symbol control messages, layers, etc.
	// FUTURE: make this thread - safe to prevent race conditions with Render.

	// For now, let's keep it as simple as possible and have the polygons parameter simply replace what was previously stored. 
	polygons_ = polygons;
}
