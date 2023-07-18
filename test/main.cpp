#include <GL/gl3w.h>
#include <SDL3/SDL.h>
#define SK_GANESH
#define SK_GL
#include <skia/include/core/SkCanvas.h>
#include <skia/include/gpu/gl/GrGLInterface.h>
#include <skia/include/gpu/GrDirectContext.h>
#include <skia/include/core/SkSurface.h>
#include <skia/include/core/SkTextBlob.h>
#include <skia/include/core/SkFont.h>
#include <skia/include/core/SkFontMetrics.h>
#include <skia/include/core/SkColor.h>
#include <skia/include/core/SkColorSpace.h>
#include "day.hpp"
#include "button.hpp"
#include "label.hpp"
#include <iostream>
using namespace std;

int main()
{

	// SDL OpenGL Config. DONE
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);


	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,0);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE,8);

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL,1);
	
	uint32_t flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE |SDL_WINDOW_HIGH_PIXEL_DENSITY;
	// SDL Video and Input Initialization. DONE
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)!=0)
		cout<<"Failed to initialize SDL"<<endl;
	
	// Window object created. DONE
	SDL_Window* window = SDL_CreateWindow("Time Man.", 640, 480, flags);

	if(!window)
		cout<<"Failed to create SDL window"<<endl;

	SDL_GLContext context=nullptr;
	context = SDL_GL_CreateContext(window);
	if(SDL_GL_MakeCurrent(window, context)!=0)
		cout<<"Failed to make context current"<<endl;

	 

	// Init openGL
	gl3wInit();

	float dpi = SDL_GetWindowPixelDensity(window);
	
	// Custom OpenGL settings
	glViewport(0, 0, 640*dpi,480*dpi);
	glClearColor(1, 1, 1, 1);
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// Skia Settings
	uint32_t format = SDL_GetWindowPixelFormat(window); // IGNORE FOR NOW

	int context_type;	
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,&context_type); // IGNORE FOR NOW
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

	clock_t t=clock();
	GrBackendRenderTarget backend_render_target(640*dpi,480*dpi,0,8,framebuffer_info);
		SkSurface* surface = SkSurface::MakeFromBackendRenderTarget(gl_context, backend_render_target, kBottomLeft_GrSurfaceOrigin, color_type,nullptr,nullptr).release();

	SkCanvas* canvas = surface->getCanvas();
	bool render=true;

	// Runtime object init;
	SDL_Event e;
	SkPaint p;
	p.setColor(SK_ColorBLACK);
	p.setStyle(SkPaint::kFill_Style);
	p.setAntiAlias(true);
	p.setDither(true);
	p.setStrokeWidth(0);
	SkFont f=SkFont(SkTypeface::MakeDefault(), 14*dpi);
	Month m(30,6,2023);
	Button b(80,700,300,100,5,"Create 30 min. task");
	Label l1(600,600,"Deadline", SK_ColorRED);
	Label l2(600,660,"Button that adds a task into calendar", SK_ColorGREEN);
	bool active=true;
	
	// main loop
	while(1)
	{
		// render
		if(render)
		{
			canvas->clear(SkColorSetRGB(255,255,255));
			m.renderMonth(canvas,&p,&f);
			b.renderButton(canvas,&p,&f);
			l1.renderLabel(canvas,&p,&f);
			l2.renderLabel(canvas,&p,&f);
			canvas->flush();
			SDL_GL_SwapWindow(window);
			render=false;
		}
		// poll events
		if(SDL_WaitEvent(&e)!=0)
		{
			if(e.type==SDL_EVENT_QUIT)
				break;
			else if(e.type==SDL_EVENT_MOUSE_BUTTON_DOWN&&active)
			{
				float mouseX,mouseY;
				SDL_GetMouseState(&mouseX,&mouseY);
				mouseX*=dpi;
				mouseY*=dpi;
				m.pollDayPress(mouseX,mouseY,&render);
				if(b.pollPress(mouseX,mouseY))
				{
					m.scheduleTask();
					render=true;
				}
				active=false;
			}
			else if(e.type==SDL_EVENT_MOUSE_BUTTON_UP)
				active=true;
		}
	}

	// clean up
	if(context)
		SDL_GL_DeleteContext(context);

	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}
