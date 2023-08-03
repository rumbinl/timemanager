#include <TM_UI.hpp>

TM_Button::TM_Button(std::string text, SkScalar x, SkScalar y, SkScalar width, SkScalar height, TM_ViewSetting viewSettings) : TM_TextView(text, width, height, x,y,viewSettings)
{
    this->select = false;
}

bool TM_Button::PollEvent(float mouseX, float mouseY)
{
    if(this->bounds.contains(mouseX,mouseY))
    {
        if(!select)
        {
            this->select = true;
            std::swap(this->viewSetting.backgroundColor, this->viewSetting.textColor);
            return true;
        }
    }
    else if(this->select)
    {
        std::swap(this->viewSetting.backgroundColor, this->viewSetting.textColor);
        this->select = false;
        return true;
    }
    return false;
}

SkScalar TM_Button::getX()
{
    return this->bounds.x();
}

void TM_Button::setX(SkScalar x)
{
    this->bounds.offsetTo(x,this->bounds.y());
}

void TM_Button::setY(SkScalar y)
{
    this->bounds.offsetTo(this->bounds.x(),y);
}