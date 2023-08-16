#include <TM_ApplicationManager.hpp>

TM_ApplicationManager::TM_ApplicationManager() : window_ptr("Timeman", 640, 480)
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

    this->calendar_view = new TM_CalendarMonthView(0, 0, 1,2024,640,480);
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

    this->calendar_view->Render(this->skia_canvas, this->skia_fontList[this->defaultFont]);

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
        else if(this->SDL_event_ptr.type== SDL_EVENT_MOUSE_MOTION || (this->SDL_event_ptr.type == SDL_EVENT_MOUSE_BUTTON_DOWN && !this->pressed))
        {
            float mouseX,mouseY;
            int leftPressed = SDL_GetMouseState(&mouseX,&mouseY)&1;
            if(this->calendar_view->PollEvents(mouseX*this->window_ptr.getDPI(),mouseY*this->window_ptr.getDPI(),leftPressed))
                should_render_update = true;
            if(this->SDL_event_ptr.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
                pressed = true;
        }
        else
            this->pressed = false;
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