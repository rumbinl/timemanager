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

const uint32_t default_SDL_window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY;

const uint32_t default_SDL_flags = SDL_INIT_VIDEO | SDL_INIT_EVENTS;

TM_Graphics_Window::TM_Graphics_Window(std::string window_title, uint32_t init_window_width, uint32_t init_window_height)
{
	this->init_success = true;
	this->SDL_window_flags =  default_SDL_window_flags;	
	this->window_title = window_title;
	this->window_width = init_window_width;
	this->window_height = init_window_height;

	if(SDL_Init(default_SDL_flags)!=0)
	{
		std::cout<<"Failed to initialize SDL! Aborting..."<<std::endl;
		this->init_success=false;
	}

	this->SDL_window_ptr = SDL_CreateWindow(this->window_title.c_str(), this->window_width, this->window_height, this->SDL_window_flags);

	if(!this->SDL_window_ptr)
	{
		std::cout<<"Failed to create SDL window! Aborting..."<<std::endl;
		this->init_success=false;
	}	

	this->SDL_GL_context = SDL_GL_CreateContext(this->SDL_window_ptr);
	// LEFT OFF
}

SkCanvas* TM_Graphics_Window::Get_skia_canvas_ptr()
{
	this->skia_window_surface = SkSurface::MakeFromBackendRenderTarget(this->skia_GL_context, *this->skia_backend_render_target, kBottomLeft_GrSurfaceOrigin, this->skia_color_type, nullptr, nullptr).release();
	return this->skia_window_surface->getCanvas();
}

TM_Graphics_Window::~TM_Graphics_Window()
{
}

void TM_Graphics_Window::Init_skia()
{
	this->skia_GL_context = GrDirectContext::MakeGL(GrGLMakeNativeInterface()).release();

	if(!this->skia_GL_context)
	{
		std::cout<<"Failed to create Skia GL context! Aborting..."<<std::endl;
		this->init_success=false;
	}

	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &this->gl_framebuffer_id);

	this->skia_GL_framebuffer_info.fFBOID = this->gl_framebuffer_id;
	this->skia_GL_framebuffer_info.fFormat = GL_RGB8;

	this->skia_color_type = kRGB_888x_SkColorType;

	this->skia_backend_render_target = new GrBackendRenderTarget(this->window_width*this->SDL_window_DPI, this->window_height*this->SDL_window_DPI, 0, 8, this->skia_GL_framebuffer_info);
}

void TM_Graphics_Window::Set_SDL_GL_attributes()
{
	for(auto setting:SDL_GL_setting_array)
		SDL_GL_SetAttribute(setting.SDL_GL_attribute, setting.setting);
}

void TM_Graphics_Window::Handle_resize(SDL_Event* window_event)
{
}
