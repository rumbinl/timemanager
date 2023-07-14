#pragma once

#include <GL/gl3w.h>

#include <SDL3/SDL.h>

#include <string>

#define SK_GANESH
#define SK_GL
#include <skia/include/core/SkCanvas.h>
#include <skia/include/gpu/gl/GrGLInterface.h>
#include <skia/include/gpu/GrDirectContext.h>
#include <skia/include/core/SkColor.h>

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
		TM_Graphics_Window(std::string window_title, uint32_t init_window_width, uint32_t init_window_height);
		void Poll_event(SDL_Event* window_event);
		SkCanvas* Get_skia_canvas_ptr();
		~TM_Graphics_Window();
	private:
		void Set_SDL_GL_attributes();
		void Handle_resize(SDL_Event* window_event);

		// General SDL Window + GL Objects
		SDL_Window* SDL_window_ptr;
		SDL_GLContext* SDL_GL_context;

		// Window attributes
		float SDL_window_DPI;
		std::string window_title;
		uint32_t window_width,window_height;
		uint32_t gl_framebuffer_id;
	
		// Skia objects
		GrDirectContext* skia_GL_context;
		GrGLFramebufferInfo skia_GL_framebuffer_info;
		SkColorType skia_color_type;
		SkSurface* skia_window_surface;
};
