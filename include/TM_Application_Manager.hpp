#pragma once

#include <TM_Core.hpp>
#include <TM_Window.hpp>

class TM_Application_Manager
{
	public:

		TM_Application_Manager();

		void Run();
		void PollEvents();
        void Render();

		~TM_Application_Manager();

	private:

        // Pointers to Timeman classes
		TM_Graphics_Window* window_ptr;

        // External API object pointers
		SDL_Event* SDL_event_ptr;
		SkCanvas* skia_canvas;

        // Runtime attributes
		bool should_render_update, should_terminate;
        SkColor skia_canvas_clear_color;
};