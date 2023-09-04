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
	skia_canvas->setMatrix(SkMatrix::Translate(this->bounds.x(), this->bounds.y()-this->yOffset));

    SkScalar y = this->viewSetting.padding;

    for(TM_RenderObject* renderObject : this->renderObjects)
    {
        renderObject->setBounds(SkRect::MakeXYWH(this->viewSetting.padding, y, this->bounds.width()-this->viewSetting.padding*2, renderObject->getBounds().height()));
        renderObject->Render(skia_canvas, font);

        y += renderObject->getBounds().height() + this->viewSetting.padding;
    }

    this->srcBounds.setWH(this->bounds.width(), y);

    skia_canvas->restoreToCount(restore);
}

bool TM_View::PollEvents(SkScalar mouseX, SkScalar mouseY, SkScalar scrollX, SkScalar scrollY, bool pressed, bool held)
{
    bool select = false;
    if(this->bounds.contains(mouseX,mouseY))
    {
        for(TM_RenderObject* renderObject : this->renderObjects)
        {
            select = select || renderObject->PollEvents(mouseX-this->bounds.x(),mouseY-this->bounds.y()+this->yOffset,scrollX,scrollY,pressed,held);
        }
        if(!select)
        {
            SkScalar newY = this->yOffset+scrollY;
            this->yOffset = fmin(fmax(0, newY),fmax(0, this->srcBounds.height()-this->bounds.height()));
            select = true;
        }
    }
    return select;
}

TM_View::~TM_View()
{

}