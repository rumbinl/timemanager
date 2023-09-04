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

    SkScalar yOffset = 0.0f;

    for(TM_RenderObject* renderObject : this->renderObjects)
    {
        std::cout<<"Render at "<<this->bounds.x()<<','<<this->bounds.y()<<std::endl;
        renderObject->setBounds(SkRect::MakeXYWH(0, yOffset, this->bounds.width(), renderObject->getBounds().height()));
        renderObject->Render(skia_canvas, font);
    }

    skia_canvas->restoreToCount(restore);
}

void TM_View::PollEvents(SkScalar mouseX, SkScalar mouseY, SkScalar scrollX, SkScalar scrollY, bool pressed)
{

}

TM_View::~TM_View()
{

}