#include <TM_UI.hpp>

TM_Button::TM_Button(std::string text, SkRect bounds, TM_ViewSetting viewSettings) : TM_TextView(text, bounds, viewSettings)
{
    this->select = false;
}

bool TM_Button::PollEvents(SkScalar mouseX, SkScalar mouseY, SkScalar scrollX, SkScalar scrollY, bool pressed, bool held)
{
    return this->bounds.contains(mouseX,mouseY);
}

SkScalar TM_Button::getX()
{
    return this->bounds.x();
}


void TM_Button::invertColors()
{
    std::swap(this->viewSetting.backgroundColor, this->viewSetting.textColor);
}