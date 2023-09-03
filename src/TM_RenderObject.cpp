#include <TM_UI.hpp>

TM_RenderObject::TM_RenderObject(SkRect bounds)
{
    this->bounds = bounds;
}

void TM_RenderObject::Render(SkCanvas* skia_canvas, SkFont* font)
{

}

bool TM_RenderObject::PollEvents(SkScalar mouseX, SkScalar mouseY, SkScalar scrollX, SkScalar scrollY, bool pressed)
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

TM_RenderObject::~TM_RenderObject()
{

}