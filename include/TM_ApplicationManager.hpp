#pragma once

#include <TM_Core.hpp>
#include <TM_Window.hpp>
#include <TM_View.hpp>
#include <TM_CalendarView.hpp>
#include <TM_TaskView.hpp>
#include <include/core/SkRefCnt.h>
#include <include/core/SkFontMgr.h>
#include <TM_Database.hpp>
#include <TM_FileDrop.hpp>
#include <map>

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
		TM_View* mainView;
		/*TM_CalendarView* calendarView;
		TM_TaskView* taskView;*/

        // Pointers to Timeman classes
		TM_Graphics_Window window_ptr =TM_Graphics_Window("Timeman", 1080, 540);

        // External API object pointers
		SDL_Event SDL_event_ptr;
		SkPaint paint;
		SkCanvas* skia_canvas;

        // Runtime attributes
		bool should_render_update=true, should_terminate=false, pressed=false;
		std::map<std::chrono::year_month_day,int> freeTimeMap;
		TM_TaskManager* taskManPtr;
		TM_TaskView* taskViewPtr;
		TM_StorageManager* storageManPtr;
        SkColor skia_canvas_clear_color;
};
