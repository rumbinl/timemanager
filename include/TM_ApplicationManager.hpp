#pragma once

#include <TM_Core.hpp>
#include <TM_Window.hpp>
#include <TM_UI.hpp>

class TM_ApplicationManager
{
	public:

		TM_ApplicationManager();

		void LoadFont(std::string fontPath);
		void setDefaultFont(int newDefaultFont);
		int getDefaultFont();
		void Run();

		~TM_ApplicationManager();

	private:
		void PollEvents();
        void Render();

		// Font and UI related variables
		std::vector<SkFont*> skia_fontList;
		int defaultFont=-1;
		TM_View* testView;
		TM_CalendarView* testCalendarView;

        // Pointers to Timeman classes
		TM_Graphics_Window window_ptr;

        // External API object pointers
		SDL_Event SDL_event_ptr;
		SkPaint paint;
		SkCanvas* skia_canvas;

        // Runtime attributes
		bool should_render_update, should_terminate, pressed=false;
        SkColor skia_canvas_clear_color;
};
