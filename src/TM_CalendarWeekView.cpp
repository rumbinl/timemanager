#include <TM_UI.hpp>

TM_CalendarWeekView::TM_CalendarWeekView(SkScalar x, SkScalar y, SkScalar width, SkScalar height, SkScalar hourHeight, TM_ViewSetting viewSettings) : TM_CalendarDayView(x,y,width,height,hourHeight,viewSettings)
{

}

void TM_CalendarWeekView::Render(SkCanvas* skia_canvas, SkFont* font)
{
    int numDays = 7;
    TM_CalendarDayView::Render(skia_canvas, font);
    SkScalar dayWidth = (this->bounds.width() - this->xOff)/((SkScalar)numDays);
    SkScalar x = this->bounds.x() + this->xOff;
    SkPaint paint;
    for(int i=0;i<numDays;i++)
    {
        x+=dayWidth;
        paint.setStyle(SkPaint::kStrokeAndFill_Style);
        paint.setColor(this->viewSettings.borderColor);
        skia_canvas->drawLine(x, this->bounds.y(), x, this->bounds.y() + this->bounds.height(),paint);
    }
    int startDayIdx=(int)floor((this->pressWeekIndexStart-this->bounds.x()-this->xOff)/dayWidth),
        endDayIdx = (int)floor((this->pressWeekIndexEnd-this->bounds.x()-this->xOff)/dayWidth);
    SkScalar firstY=pressDayIndexStart,secondY=pressDayIndexEnd;
    if(startDayIdx>endDayIdx) { std::swap(startDayIdx, endDayIdx); std::swap(firstY,secondY); };
    SkScalar startDayX = this->bounds.x()+xOff+dayWidth*startDayIdx
    ,endDayX = this->bounds.x()+xOff+dayWidth*endDayIdx;
    SkScalar topY=this->yOff -this->scrollY+ (this->hourHeight/12.0f)*floor((firstY-this->yOff)/(this->hourHeight/12.0f)),
    botY = this->yOff -this->scrollY + (this->hourHeight/12.0f)*floor((secondY-this->yOff)/(this->hourHeight/12.0f)),
            height = (this->hourHeight/12.0f)*floor((secondY-firstY)/(this->hourHeight/12.0f));
            paint.setColor(this->viewSettings.backgroundColor);
    if(startDayIdx == endDayIdx)
    {
		SkRRect rect = SkRRect::MakeRectXY(SkRect::MakeXYWH(startDayX, topY, dayWidth, height),20,20);
        skia_canvas->drawRRect(rect, paint);
    }
    else 
    {
		SkRRect rect = SkRRect::MakeRectXY(SkRect::MakeXYWH(startDayX, topY, dayWidth, this->bounds.y()+this->bounds.height()-topY),20,20);
        skia_canvas->drawRRect(rect, paint);
        SkRRect coverDays = SkRRect::MakeRectXY(SkRect::MakeXYWH(endDayX, this->bounds.y(), dayWidth, botY-this->bounds.y()),20,20);
        skia_canvas->drawRRect(coverDays, paint);
        skia_canvas->drawRRect(SkRRect::MakeRectXY(SkRect::MakeXYWH(startDayX+dayWidth, this->bounds.y(), dayWidth*(endDayIdx-startDayIdx-1), this->bounds.height()),20,20), paint);
    }
}

bool TM_CalendarWeekView::PollEvents(float x, float y, float scrollY, bool pressed)
{
    if(this->bounds.contains(x,y))
    {
        if(scrollY!=0)
		{
			this->scrollY+=scrollY;
			SkScalar scrollLimitY = this->srcBounds.height()-this->bounds.height();
			this->scrollY = fmin(scrollLimitY, fmax(0, this->scrollY));
        }
        if(pressed&&this->selected == false)
		{
			this->pressDayIndexStart = y+this->scrollY;
            this->pressWeekIndexStart = x;
		}
        this->selected = pressed;

		if(pressed)
        {
			this->pressDayIndexEnd = y+this->scrollY;
            this->pressWeekIndexEnd = x;
        }

		return true;
    }
    if(this->selected && !pressed)
	{
		this->selected = false;
		this->pressDayIndexStart = 
		this->pressDayIndexEnd = 
        this->pressWeekIndexStart = 
        this->pressWeekIndexEnd = -1;
		return true;
	}
    return false;
}

TM_CalendarWeekView::~TM_CalendarWeekView()
{

}