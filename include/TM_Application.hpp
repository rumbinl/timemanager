#pragma once

#include <GL/gl3w.h>

#include <SDL3/SDL.h>

#include <iostream>

#include <string>

#define SK_GANESH
#define SK_GL
#include <skia/include/core/SkCanvas.h>
#include <skia/include/gpu/gl/GrGLInterface.h>
#include <skia/include/gpu/GrDirectContext.h>
#include <skia/include/core/SkSurface.h>
#include <skia/include/core/SkColor.h>
#include <skia/include/core/SkColorSpace.h>

// Naming Convention
// Words spaced with '_'
// Abbreviations fully capitalized
// Functions' first character capitalized

/* 

-----Window Objects------

1. SDL_Window SDL_Window_ptr 

2. uint32_t SDL_Window_flags

3. SDL_GLContext SDL_gl_context  

4. float DPI

5. int Window_Width

6. int Window_Height

*/

typedef struct { SDL_GLattr SDL_GL_attribute; uint32_t setting; } SDL_GL_setting;

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
