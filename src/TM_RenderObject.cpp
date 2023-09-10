#include <TM_UI.hpp>

TM_RenderObject::TM_RenderObject(SkRect bounds, TM_ViewSetting viewSetting)
{
    this->bounds = bounds;
    this->viewSetting = viewSetting;
}

void TM_RenderObject::Render(SkCanvas* skia_canvas, SkFont* font)
{

}

bool TM_RenderObject::PollEvents(TM_EventInput eventInput)
{
    return this->bounds.contains(eventInput.mouseX,eventInput.mouseY);
}

SkRect TM_RenderObject::getBounds()
{
    return this->bounds;
}

SkRect TM_RenderObject::getSrcBounds()
{
    return this->srcBounds;
}

void TM_RenderObject::setBounds(SkRect newBounds)
{
    this->bounds = newBounds;
}

bool TM_RenderObject::exists()
{
    return this->existence;
}

void TM_RenderObject::setExistence(bool existence)
{
    this->existence = existence;
}
