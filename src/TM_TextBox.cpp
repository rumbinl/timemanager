#include <TM_Core.hpp>
#include <TM_UI.hpp>

TM_TextBox::TM_TextBox(SkRect bounds, std::string placeholder="", TM_ViewSetting viewSetting) : TM_TextView(placeholder, bounds, viewSetting, false)
{
    this->placeholder = placeholder;
}

void TM_TextBox::Render(SkCanvas* skia_canvas, SkFont* font)
{
    if(!this->fitted)
    {
        TM_TextView::setHeightFont(font);
        this->fitted = true;
    }
    if(this->content == "")
        TM_TextView::setColorOpacity(150);
    else
        TM_TextView::setColorOpacity(255);
    TM_TextView::Render(skia_canvas, font);
}

bool TM_TextBox::PollEvents(SkScalar mouseX, SkScalar mouseY, SkScalar scrollX, SkScalar scrollY, bool pressed)
{
    if(TM_TextView::getBounds().contains(mouseX,mouseY)) 
    {
        if(scrollX != 0)
        {
            TM_TextView::setTextXOffset(scrollX);
            return true;
        }
    }
    return false;
}