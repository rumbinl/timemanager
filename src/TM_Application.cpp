#include "TM_Application.hpp"

// All necessary Skia GL settings
const SDL_GL_setting SDL_GL_setting_array[10]={{SDL_GL_CONTEXT_MAJOR_VERSION, 3},
					       {SDL_GL_CONTEXT_MINOR_VERSION, 0},
					       {SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE},
					       {SDL_GL_RED_SIZE,8},
					       {SDL_GL_GREEN_SIZE,8},
					       {SDL_GL_BLUE_SIZE,8},
					       {SDL_GL_DOUBLEBUFFER,1},
					       {SDL_GL_DEPTH_SIZE,0},
					       {SDL_GL_STENCIL_SIZE,8},
					       {SDL_GL_ACCELERATED_VISUAL,1}};

TM_Graphics_Window::TM_Graphics_Window(std::string window_title, uint32_t init_window_width, uint32_t init_window_height)
{
	
}

void TM_Graphics_Window::Poll_event(SDL_Event* window_event)
{
}

SkCanvas* TM_Graphics_Window::Get_skia_canvas_ptr()
{
}

TM_Graphics_Window::~TM_Graphics_Window()
{
}

void TM_Graphics_Window::Set_SDL_GL_attributes()
{
	for(auto setting:SDL_GL_setting_array)
		SDL_GL_SetAttribute(setting.SDL_GL_attribute, setting.setting);
}

void TM_Graphics_Window::Handle_resize(SDL_Event* window_event)
{
}
