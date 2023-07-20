#include <TM_Application_Manager.hpp>

TM_Application_Manager::TM_Application_Manager()
{
    this->window_ptr = new TM_Graphics_Window("Timeman", 640, 480);

    if(!this->window_ptr->Was_init_success())
    {
        std::cout<<"Window failed to initialize! Aborting..."<<std::endl;
        this->should_terminate = true;
        return;
    }

    this->skia_canvas = this->window_ptr->Get_skia_canvas_ptr();

    this->should_render_update = true;
    this->skia_canvas_clear_color = SkColorSetARGB(255,255,255,255);
}

void TM_Application_Manager::Run()
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

void TM_Application_Manager::Render()
{
    this->skia_canvas->clear(this->skia_canvas_clear_color);

    this->skia_canvas->flush();
    this->window_ptr->Swap_buffers();
}

void TM_Application_Manager::PollEvents()
{

}

~TM_Application_Manager::TM_Application_Manager()
{
    delete this->window_ptr;
}