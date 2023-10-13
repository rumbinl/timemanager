#include <TM_TimeDial.hpp>

TM_TimeDial::TM_TimeDial(SkRect bounds, TM_ViewSetting viewSetting) : TM_RenderObject(bounds, viewSetting)
{
}

void TM_TimeDial::Render(TM_RenderInfo renderInfo)
{
    SkPaint paint;
    paint.setColor(this->viewSetting.backgroundColor);
    paint.setStyle(SkPaint::kStroke_Style);
    paint.setAntiAlias(true);
    paint.setStrokeWidth(this->dialThickness);
    renderInfo.canvas->save();
    renderInfo.canvas->clipRect(this->bounds);
    renderInfo.canvas->translate(this->bounds.x(), this->bounds.y());
    SkScalar cx = this->bounds.width()/2.0f,cy = this->bounds.height()/2.0f, length = std::fmin(this->bounds.width(), this->bounds.height())/2.0f-this->dialThickness/2.0f;
    renderInfo.canvas->drawCircle(cx,cy, length, paint);

    cx = cx + sin(2 * M_PI * dialProgressPercentage/100.0f) * length, cy = cy - length * cos(2 * M_PI * dialProgressPercentage/100.0f);
    paint.setStyle(SkPaint::kFill_Style);
    paint.setStrokeWidth(0);
    paint.setColor(this->viewSetting.textColor);
    renderInfo.canvas->drawCircle(cx,cy, this->dialThickness/2.0f, paint);
    renderInfo.canvas->restore();
}

bool TM_TimeDial::PollEvents(TM_EventInput eventInput)
{
    SkScalar length = std::fmin(this->bounds.width(), this->bounds.height())/2.0f-this->dialThickness/2.0f;
    SkScalar cx = this->bounds.width()/2.0f + sin(2 * M_PI * dialProgressPercentage/100.0f)*length, cy = this->bounds.height()/2.0f - length * cos(2 * M_PI * dialProgressPercentage/100.0f); 
    SkRect rectBounds = SkRect::MakeXYWH(cx-this->dialThickness/2.0f, cy-this->dialThickness/2.0f, this->dialThickness, this->dialThickness);
    SkRRect circleBounds = SkRRect::MakeRectXY(rectBounds, this->dialThickness/2.0f, this->dialThickness/2.0f);
    if(circleBounds.contains(SkRect::MakeXYWH(eventInput.mouseX - this->bounds.x(),eventInput.mouseY - this->bounds.y(), 1, 1)) && eventInput.mousePressed)
    {
        this->dialProgressPercentage += 1;
        return true;
    }
    return false;
}
