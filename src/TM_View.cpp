#include <TM_UI.hpp>

TM_View::TM_View(SkRect bounds, std::vector<TM_RenderObject*> objects, TM_ViewSetting viewSetting) : TM_RenderObject(bounds, viewSetting)
{
    this->renderObjects = objects;
    this->viewSetting = viewSetting;
    this->show.assign(this->renderObjects.size(),true);
}

void TM_View::Render(SkCanvas* skia_canvas, SkFont* font)
{
    SkPaint paint;
    paint.setColor(this->viewSetting.backgroundColor);
    paint.setStyle(SkPaint::kFill_Style);
    skia_canvas->drawRect(this->bounds, paint);
    paint.setColor(this->viewSetting.borderColor);
    paint.setStyle(SkPaint::kStroke_Style);
    skia_canvas->drawRect(this->bounds, paint);

    int restore = skia_canvas->save();
    skia_canvas->clipRect(this->bounds);
	skia_canvas->setMatrix(SkMatrix::Translate(this->bounds.x(), this->bounds.y()));

    SkScalar yOffset = this->viewSetting.padding;

    for(TM_RenderObject* renderObject : this->renderObjects)
    {
        renderObject->setBounds(SkRect::MakeXYWH(this->viewSetting.padding, yOffset, this->bounds.width()-this->viewSetting.padding*2, renderObject->getBounds().height()));
        renderObject->Render(skia_canvas, font);

        yOffset += renderObject->getBounds().height() + this->viewSetting.padding;
    }

    skia_canvas->restoreToCount(restore);
}

bool TM_View::PollEvents(SkScalar mouseX, SkScalar mouseY, SkScalar scrollX, SkScalar scrollY, bool pressed)
{
    bool select = false;
    if(this->bounds.contains(mouseX,mouseY))
    {
        for(TM_RenderObject* renderObject : this->renderObjects)
        {
            select = select || renderObject->PollEvents(mouseX-this->bounds.x(),mouseY-this->bounds.y(),scrollX,scrollY,pressed);
        }
    }
    return select;
}

TM_View::~TM_View()
{

}