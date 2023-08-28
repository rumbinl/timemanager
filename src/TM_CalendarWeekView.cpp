#include <TM_UI.hpp>

TM_CalendarWeekView::TM_CalendarWeekView(SkScalar x, SkScalar y, SkScalar width, SkScalar height, SkScalar hourHeight, TM_ViewSetting viewSettings) : TM_CalendarDayView(x,y,width,height,hourHeight,viewSettings)
{

}

void TM_CalendarWeekView::Render(SkCanvas* skia_canvas, SkFont* font)
{
    TM_CalendarDayView::Render(skia_canvas, font);
    SkScalar dayWidth = (this->bounds.width() - this->xOff)/7.0f;
    SkScalar x = this->bounds.x() + this->xOff;
    for(int i=0;i<7;i++)
    {
        x+=dayWidth;
        SkPaint paint;
        paint.setStyle(SkPaint::kStrokeAndFill_Style);
        paint.setColor(this->viewSettings.borderColor);
        skia_canvas->drawLine(x, this->bounds.y(), x, this->bounds.y() + this->bounds.height(),paint);
    }
}

bool TM_CalendarWeekView::PollEvents(float x, float y, float scrollY, bool pressed)
{
    return TM_CalendarDayView::PollEvents(x,y,scrollY,pressed);
}

TM_CalendarWeekView::~TM_CalendarWeekView()
{

}