#include <TM_ApplicationManager.hpp>

TM_ApplicationManager::TM_ApplicationManager() : window_ptr("Timeman", 960, 540)
{
	if(!this->window_ptr.Was_init_success())
	{
	std::cout<<"Window failed to initialize! Aborting..."<<std::endl;
	this->should_terminate = true;
	return;
	}

	this->skia_canvas = this->window_ptr.Get_skia_canvas_ptr();
	if(!this->skia_canvas)
	{
	std::cout<<"Failed to create Skia canvas!"<<std::endl;
	}

	this->should_render_update = true;
	this->skia_canvas_clear_color = colorScheme[BACKGROUND_COLOR_INDEX];

	this->calendar_month_view = new TM_CalendarMonthView(SkRect::MakeXYWH(0,0,640,480), 1,2024);
	this->calendar_week_view = new TM_CalendarWeekView(SkRect::MakeXYWH(640, 0, 1024, 840),3);
	this->test_text_box = new TM_TextBox(SkRect::MakeXYWH(0, 480, 100, 30), "Write something");
}

void TM_ApplicationManager::Run()
{
    while(!this->should_terminate)
    {
        if(this->should_render_update)
        {
            this->Render();
            this->should_render_update = false;
        }
        this->PollEvents();
    }
}

void TM_ApplicationManager::Render()
{
	this->skia_canvas->resetMatrix();
	this->skia_canvas->clear(this->skia_canvas_clear_color);

	this->calendar_month_view->Render(this->skia_canvas, this->skia_fontList[this->defaultFont]);
	this->calendar_week_view->Render(this->skia_canvas, this->skia_fontList[this->defaultFont]);
	this->test_text_box->Render(this->skia_canvas, this->skia_fontList[this->defaultFont]);

	this->skia_canvas->flush();
	this->window_ptr.Swap_buffers();
}

void TM_ApplicationManager::PollEvents()
{
    if(SDL_WaitEvent(&this->SDL_event_ptr))
    {
		
		if(this->SDL_event_ptr.type==SDL_EVENT_QUIT)
			this->should_terminate=true;
		else if(this->SDL_event_ptr.type == SDL_EVENT_WINDOW_RESIZED)
		{
			this->window_ptr.Handle_resize(&this->SDL_event_ptr);
			this->skia_canvas = this->window_ptr.Get_skia_canvas_ptr();
			this->should_render_update = true;
		}
		else if(this->SDL_event_ptr.type == SDL_EVENT_KEY_DOWN)
		{
		}
		else
		{
			float mouseX,mouseY,scrollY=0.0f,scrollX=0.0f;
			bool pressed = SDL_event_ptr.type==SDL_EVENT_MOUSE_BUTTON_DOWN;
			SDL_PumpEvents(); 
			bool held = (SDL_GetMouseState(&mouseX,&mouseY)&1)>0; 
			if(this->SDL_event_ptr.type == SDL_EVENT_MOUSE_WHEEL)
				scrollY = this->SDL_event_ptr.wheel.y, scrollX = this->SDL_event_ptr.wheel.x;
			if( this->calendar_month_view->PollEvents(
					mouseX*this->window_ptr.getDPI(),
					mouseY*this->window_ptr.getDPI(),
					scrollX*scrollSensFactor,
					scrollY*scrollSensFactor*(this->SDL_event_ptr.wheel.direction==SDL_MOUSEWHEEL_FLIPPED?-1:1),
					pressed) 
								||
				this->calendar_week_view->PollEvents(
					mouseX*this->window_ptr.getDPI(),
					mouseY*this->window_ptr.getDPI(),
					scrollX*scrollSensFactor,
					scrollY*scrollSensFactor*(this->SDL_event_ptr.wheel.direction==SDL_MOUSEWHEEL_FLIPPED?-1:1),
					pressed||held)
								||
				this->test_text_box->PollEvents(
					mouseX*this->window_ptr.getDPI(), 
					mouseY*this->window_ptr.getDPI(), 
					scrollX*scrollSensFactor,
					scrollY*scrollSensFactor*(this->SDL_event_ptr.wheel.direction==SDL_MOUSEWHEEL_FLIPPED?-1:1),
					pressed)
				)
				should_render_update = true;
		}
    }
}

void TM_ApplicationManager::LoadFont(std::string fontPath)
{
    this->skia_fontList.push_back(new SkFont(SkTypeface::MakeFromFile(fontPath.c_str()))); 
}

void TM_ApplicationManager::setDefaultFont(int newDefaultFont)
{
    if(newDefaultFont >= this->skia_fontList.size())
        this->defaultFont = 0;
    else
        this->defaultFont = newDefaultFont;
}

int TM_ApplicationManager::getDefaultFont()
{
    return this->defaultFont;
}

TM_ApplicationManager::~TM_ApplicationManager()
{
}
