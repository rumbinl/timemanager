#include <TM_UI.hpp>

TM_Button::TM_Button(std::string text, SkRect bounds, void (*actionFunc)()) : TM_TextView(text, bounds)
{
    this->actionFunc = actionFunc;
}

bool TM_Button::PollEvents(TM_EventInput eventInput) 
{
    if(this->bounds.contains(eventInput.mouseX, eventInput.mouseY) && eventInput.mousePressed)
    {
        if(actionFunc != NULL)
            (*actionFunc)();
    }
}

void TM_Button::setActionFunc(void (*actionFunc)())
{
    this->actionFunc = actionFunc;
}

TM_Button::~TM_Button()
{

}