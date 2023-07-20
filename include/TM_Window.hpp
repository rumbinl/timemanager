#pragma once

#include <TM_Core.hpp>

// Naming Convention
// Words spaced with '_'
// Abbreviations fully capitalized
// Functions' first character capitalized

typedef struct { SDL_GLattr SDL_GL_attribute; int setting; } SDL_GL_setting;

class TM_Graphics_Window
/*
	Contains all functions and variables 

	absolutely necessary for the creation 

	and bare minimum maintenance of an

	SDL-Skia-GL window.
*/
{
	public:

		TM_Graphics_Window(std::string window_title, uint32_t init_window_width, uint32_t init_window_height); // Constructor

		SkCanvas* Get_skia_canvas_ptr(); // Gets Skia Canvas object for drawing 
		bool Was_init_success();
		void Swap_buffers();

		~TM_Graphics_Window();

	private:
		void Init_skia(); // Helper function of constructor
		void Destroy_skia(); // Helper function of destructor
		void Set_SDL_GL_attributes(); // Sets API specific GL attributes
		void Handle_resize(SDL_Event* window_event); // Manipulates window data in case of resize

		// General SDL Window + GL Objects
		bool init_success;
		SDL_Window* SDL_window_ptr;
		SDL_GLContext SDL_GL_context;

		// Window attributes
		float SDL_window_DPI;
		uint32_t SDL_window_flags;
		std::string window_title;
		uint32_t window_width,window_height;
		GLint gl_framebuffer_id;
	
		// Skia objects
		GrDirectContext* skia_GL_context=NULL;
		GrBackendRenderTarget* skia_backend_render_target=NULL;
		GrGLFramebufferInfo skia_GL_framebuffer_info;
		SkColorType skia_color_type;
		SkSurface* skia_window_surface;
};