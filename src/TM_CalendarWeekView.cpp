#include <TM_UI.hpp>

TM_CalendarWeekView::TM_CalendarWeekView(SkScalar x, SkScalar y, SkScalar width, SkScalar height, int numDays, SkScalar hourHeight, TM_ViewSetting viewSettings)
{
    this->bounds = SkRect::MakeXYWH(x,y,width,height);
	this->viewSettings = viewSettings;
	this->hourHeight = hourHeight;
	this->scrollY = 0;
	this->srcBounds = this->bounds;
    this->numDays = numDays;
}

void TM_CalendarWeekView::RenderTimes(SkCanvas* skia_canvas, SkFont* font)
{
    SkPaint paint;
	font->setSize(this->viewSettings.fontSize);
	SkFontMetrics fontMetrics;
	font->getMetrics(&fontMetrics);
	SkRect text_bounds;
	font->measureText("XX:XX", 5*sizeof(char), SkTextEncoding::kUTF8, &text_bounds, &paint);
	this->xOff = text_bounds.width();
	paint.setColor(this->viewSettings.borderColor);

	SkScalar yOff = -this->scrollY, y = 0.0f;

	for(int i=0;i<24;i++)
	{
		std::string timeString = std::to_string(i)+":00";
		font->measureText(timeString.c_str(), timeString.size()*sizeof(char), SkTextEncoding::kUTF8, &text_bounds, &paint);

		if(!i)
		{
			this->viewSettings.padding = text_bounds.height()-fontMetrics.fBottom;
			y += this->viewSettings.padding;
		}

		skia_canvas->drawString(timeString.c_str(), xOff-text_bounds.width(), y+yOff+fontMetrics.fBottom, *font, paint);
		skia_canvas->drawLine(xOff, y+yOff, this->bounds.width(), y+yOff, paint);
		y += this->hourHeight;
	}
	this->srcBounds.setWH(this->srcBounds.width(), y);
    SkScalar dayWidth = (this->bounds.width() - this->xOff)/((SkScalar)this->numDays);
    for(int i=1;i<=numDays;i++)
    {
        SkScalar x = xOff + i*dayWidth;
        skia_canvas->drawLine(x, 0, x, this->bounds.height(),paint);
    }
}

void TM_CalendarWeekView::Render(SkCanvas* skia_canvas, SkFont* font)
{
    SkPaint paint;
	paint.setStyle(SkPaint::kStroke_Style);
	paint.setColor(this->viewSettings.borderColor);

	skia_canvas->drawRect(this->bounds, paint);

	paint.setStyle(SkPaint::kStrokeAndFill_Style);

	skia_canvas->save();
	skia_canvas->clipRect(this->bounds);
	skia_canvas->setMatrix(SkMatrix::Translate(this->bounds.x(), this->bounds.y()));

    RenderTimes(skia_canvas, font);
    
    SkScalar dayWidth = (this->bounds.width() - this->xOff)/((SkScalar)this->numDays);

    if(pressWeekIndexStart == -1) 
    { 
        skia_canvas->restore();
        return;
    }
    int startDayIdx=(int)floor((this->pressWeekIndexStart-this->xOff)/dayWidth),
        endDayIdx = (int)floor((this->pressWeekIndexEnd-this->xOff)/dayWidth);


    SkScalar firstY=pressDayIndexStart,secondY=pressDayIndexEnd;

    if(startDayIdx>endDayIdx) { std::swap(startDayIdx, endDayIdx); std::swap(firstY,secondY); }

    startDayIdx = std::min(std::max(startDayIdx,0),this->numDays);
    endDayIdx = std::min(std::max(endDayIdx,0),this->numDays);

    SkScalar startDayX = this->xOff+dayWidth*startDayIdx
            ,endDayX   = this->xOff+dayWidth*endDayIdx;

    SkScalar timeStep = this->hourHeight/12.0f;
    SkScalar topY = this->viewSettings.padding - this->scrollY + (timeStep)*floor((firstY  - this->yOff)/(timeStep)),
             botY = this->viewSettings.padding - this->scrollY + (timeStep)*floor((secondY - this->yOff)/(timeStep));

    paint.setColor(this->viewSettings.backgroundColor);
    paint.setAlpha(100);

    if(startDayIdx == endDayIdx)
    {
		SkRRect rect = SkRRect::MakeRectXY(SkRect::MakeXYWH(startDayX, topY, dayWidth, botY-topY),20,20);
        skia_canvas->drawRRect(rect, paint);
    }
    else 
    {
		SkRRect startDay = SkRRect::MakeRectXY(SkRect::MakeXYWH(startDayX, topY, dayWidth, this->bounds.height()-topY),20,20);
        skia_canvas->drawRRect(startDay, paint);
        SkRRect coverDays = SkRRect::MakeRectXY(SkRect::MakeXYWH(endDayX, 0, dayWidth, botY),20,20);
        skia_canvas->drawRRect(coverDays, paint);
        SkRRect endDay = SkRRect::MakeRectXY(SkRect::MakeXYWH(startDayX+dayWidth, 0, dayWidth*(endDayIdx-startDayIdx-1), this->bounds.height()),20,20);
        skia_canvas->drawRRect(endDay, paint);
    }
    skia_canvas->restore();
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
            this->pressWeekIndexStart = x-this->bounds.x();
		}
        this->selected = pressed;

		if(pressed)
        {
			this->pressDayIndexEnd = y+this->scrollY;
            this->pressWeekIndexEnd = x-this->bounds.x();
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