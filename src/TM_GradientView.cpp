#include <TM_GradientView.hpp>

TM_GradientView::TM_GradientView(SkRect bounds) : TM_RenderObject(bounds)
{

}

void TM_GradientView::Render(TM_RenderInfo renderInfo)
{
    SkRect clipRect;
    if(this->bounds.width()<this->bounds.height())
        clipRect = SkRect::MakeXYWH(this->bounds.x(), this->bounds.y()+this->bounds.height()/2 - this->bounds.width()/2, this->bounds.width(), this->bounds.width());
    else
        clipRect = SkRect::MakeXYWH(this->bounds.x()+this->bounds.width()/2 - this->bounds.height()/2, this->bounds.y(), this->bounds.height(), this->bounds.height());

    renderInfo.canvas->save();
    renderInfo.canvas->clipRect(clipRect);
    renderInfo.canvas->translate(clipRect.x(), clipRect.y());

    SkColor colors[] = { 0xFF0000FF, 0xFF00FF00, 0xFFFF0000, 0xFF0000FF };
    SkScalar cx = clipRect.width()/2.0f, cy = clipRect.width()/2.0f;
    SkPaint paint;
    paint.setShader(SkGradientShader::MakeSweep(cx, cy, colors, nullptr, 4));
    paint.setAntiAlias(true);
    renderInfo.canvas->drawPaint(paint);

    renderInfo.canvas->restore();
}