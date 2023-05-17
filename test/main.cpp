#include <gl3w/GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <skia/include/core/SkCanvas.h>
#define SK_GANESH
#define SK_GL
#include <skia/include/gpu/gl/GrGLInterface.h>
#include <skia/include/gpu/GrDirectContext.h>
#include <skia/include/core/SkSurface.h>
#include <skia/include/core/SkColor.h>
#include <skia/include/core/SkColorSpace.h>

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	
	GLFWwindow* context = glfwCreateWindow(640,480,"skia",NULL,NULL);
	glfwMakeContextCurrent(context);

	gl3wInit();
	
	sk_sp<GrDirectContext> gl_context = GrDirectContext::MakeGL();
	GrGLFramebufferInfo framebuffer_info;
	framebuffer_info.fFBOID=0;
	framebuffer_info.fFormat=GL_RGBA8;
	SkColorType color_type = kRGBA_8888_SkColorType;
	GrBackendRenderTarget backend_render_target(640,480,4,8,framebuffer_info);
	sk_sp<SkSurface> surface = SkSurface::MakeFromBackendRenderTarget(gl_context.get(), backend_render_target, kBottomLeft_GrSurfaceOrigin, color_type, nullptr, nullptr);

	while(!glfwWindowShouldClose(context))
	{
		glfwSwapBuffers(context);

		surface->getCanvas()->clear(SK_ColorWHITE);
		surface->flushAndSubmit();
		
		glfwPollEvents();
	}

	glfwDestroyWindow(context);
	glfwTerminate();
	return 0;
}
