#include <GL/gl3w.h>
#include <SDL3/SDL.h>
#define SK_GANESH
#define SK_GL
#include <skia/include/core/SkCanvas.h>
#include <skia/include/gpu/gl/GrGLInterface.h>
#include <skia/include/gpu/GrDirectContext.h>
#include <skia/include/core/SkSurface.h>
#include <skia/include/core/SkFont.h>
#include <skia/include/core/SkColor.h>
#include <skia/include/core/SkColorSpace.h>
#include <iostream>
using namespace std;

int main()
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	
	SDL_GLContext context=nullptr;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	uint32_t flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,0);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE,8);

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL,1);
	
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)!=0)
		cout<<"Failed to initialize SDL"<<endl;
	
	SDL_Window* window = SDL_CreateWindow("Skia Test", 640, 480, flags);

	if(!window)
		cout<<"Failed to create SDL window"<<endl;

	context = SDL_GL_CreateContext(window);
	if(SDL_GL_MakeCurrent(window, context)!=0)
		cout<<"Failed to make context current"<<endl;

	uint32_t format = SDL_GetWindowPixelFormat(window);
	int context_type;	
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,&context_type); 

	gl3wInit();

	glViewport(0, 0, 640,480);
	glClearColor(1, 1, 1, 1);
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	GrDirectContext* gl_context = GrDirectContext::MakeGL(GrGLMakeNativeInterface()).release();
	if(gl_context==NULL)
		cout<<"Failed to create Skia GL context"<<endl;
	GrGLFramebufferInfo framebuffer_info;
	GLint buffer;
	glGetIntegerv( GL_FRAMEBUFFER_BINDING, &buffer);
	framebuffer_info.fFBOID=buffer;
	framebuffer_info.fFormat=GL_RGB8;
	SkColorType color_type = kRGB_888x_SkColorType;
	GrColorType grColorType = SkColorTypeToGrColorType(color_type);

	GrBackendRenderTarget backend_render_target(640,480,0,8,framebuffer_info);
		SkSurface* surface = SkSurface::MakeFromBackendRenderTarget(gl_context, backend_render_target, kBottomLeft_GrSurfaceOrigin, color_type,nullptr,nullptr).release();

	SkCanvas* canvas = surface->getCanvas();

	SDL_Event e;

	while(1)
	{
		SDL_PollEvent(&e);
		if(e.type==SDL_EVENT_QUIT)
			break;
		canvas->clear(SK_ColorRED);
		canvas->flush();
		SDL_GL_SwapWindow(window);
	}

	if(context)
		SDL_GL_DeleteContext(context);

	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}
