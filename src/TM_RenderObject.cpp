#include <TM_UI.hpp>

TM_RenderObject::TM_RenderObject(SkRect bounds, TM_ViewSetting viewSetting)
{
    this->bounds = bounds;
    this->viewSetting = viewSetting;
}

void TM_RenderObject::Render(SkCanvas* skia_canvas, SkFont* font)
{

}

bool TM_RenderObject::PollEvents(SkScalar mouseX, SkScalar mouseY, SkScalar scrollX, SkScalar scrollY, bool pressed, bool held)
{
    return this->bounds.contains(mouseX,mouseY);
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

TM_RenderObject::~TM_RenderObject()
{

}