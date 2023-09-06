#include <TM_UI.hpp>

TM_CalendarWeekView::TM_CalendarWeekView(SkRect bounds, std::chrono::year_month_day* focusDate, int numDays, SkScalar hourHeight, TM_ViewSetting viewSetting) : TM_RenderObject(bounds, viewSetting)
{
	this->hourHeight = hourHeight;
	this->scrollY = 0;
    this->numDays = numDays;
    this->focusDate = focusDate;
}

void TM_CalendarWeekView::RenderTimes(SkCanvas* skia_canvas, SkFont* font)
{
    SkPaint paint;
	font->setSize(this->viewSetting.fontSize);
	SkFontMetrics fontMetrics;
	font->getMetrics(&fontMetrics);
	SkRect text_bounds;
	font->measureText("XX:XX", 5*sizeof(char), SkTextEncoding::kUTF8, &text_bounds, &paint);
	this->xOff = text_bounds.width();
	paint.setColor(this->viewSetting.borderColor);

	SkScalar yOff = -this->scrollY, y = 0.0f;
    SkScalar dayWidth = (this->bounds.width()-xOff)/(SkScalar)numDays;
    
    yOff+=fontMetrics.fDescent-fontMetrics.fAscent;

	for(int i=0;i<24;i++)
	{
		std::string timeString = std::to_string(i)+":00";
		font->measureText(timeString.c_str(), timeString.size()*sizeof(char), SkTextEncoding::kUTF8, &text_bounds, &paint);

		if(!i)
		{
			this->viewSetting.padding = text_bounds.height()-fontMetrics.fBottom;
			y += this->viewSetting.padding;
		}

		skia_canvas->drawString(timeString.c_str(), xOff-text_bounds.width(), y+yOff+fontMetrics.fBottom, *font, paint);
		skia_canvas->drawLine(xOff, y+yOff, this->bounds.width(), y+yOff, paint);
		y += this->hourHeight;
	}
	this->srcBounds.setWH(this->srcBounds.width(), y);
    for(int i=1;i<numDays;i++)
    {
        SkScalar x = xOff + i*dayWidth;
        skia_canvas->drawLine(x, 0, x, this->bounds.height(),paint);
    }
}

void TM_CalendarWeekView::Render(SkCanvas* skia_canvas, SkFont* font)
{
    SkPaint paint;
	paint.setStyle(SkPaint::kStroke_Style);
	paint.setColor(this->viewSetting.borderColor);

	skia_canvas->drawRect(this->bounds, paint);

	paint.setStyle(SkPaint::kFill_Style);

    font->setSize(this->viewSetting.fontSize);
	SkFontMetrics fontMetrics;
	font->getMetrics(&fontMetrics);

    SkScalar dayWidth = (this->bounds.width() - this->xOff)/((SkScalar)this->numDays);
    SkScalar labelHeight = fontMetrics.fDescent-fontMetrics.fAscent;

    for(int i=0;i<numDays;i++)
    {
        std::chrono::year_month_day currentDate = std::chrono::sys_days{*focusDate} + std::chrono::days{i};
        std::string date = std::to_string(static_cast<unsigned>(currentDate.day()))+"/"+std::to_string(static_cast<unsigned>(currentDate.month()));
        TM_TextView::Render(date, SkRect::MakeXYWH(this->bounds.x()+xOff+i*dayWidth,this->bounds.y(),dayWidth,labelHeight),skia_canvas,font,{colorScheme[3],colorScheme[2],colorScheme[1],1,this->viewSetting.fontSize,5});
    }

	skia_canvas->save();
	skia_canvas->clipRect(SkRect::MakeXYWH(this->bounds.x(),this->bounds.y()+labelHeight,this->bounds.width(),this->bounds.height()-labelHeight));
	skia_canvas->translate(this->bounds.x(), this->bounds.y());

    RenderTimes(skia_canvas, font);
    
    if(pressWeekIndexStart == -1) 
    { 
        skia_canvas->restore();
        return;
    }
    int startDayIdx=(int)floor((this->pressWeekIndexStart-this->xOff)/dayWidth),
        endDayIdx = (int)floor((this->pressWeekIndexEnd-this->xOff)/dayWidth);


    SkScalar firstY=pressDayIndexStart,secondY=pressDayIndexEnd;

    if(startDayIdx>endDayIdx) { std::swap(startDayIdx, endDayIdx); std::swap(firstY,secondY); }

    startDayIdx = min(max(startDayIdx,0),this->numDays);
    endDayIdx = min(max(endDayIdx,0),this->numDays);

    SkScalar startDayX = this->xOff+dayWidth*startDayIdx
            ,endDayX   = this->xOff+dayWidth*endDayIdx;

    SkScalar timeStep = this->hourHeight/12.0f;
    SkScalar topY = this->viewSetting.padding - this->scrollY + (timeStep)*floor((firstY  - this->yOff)/(timeStep)),
             botY = this->viewSetting.padding - this->scrollY + (timeStep)*floor((secondY - this->yOff)/(timeStep));

    paint.setColor(this->viewSetting.textColor);

    SkScalar r = 0;
    if(startDayIdx == endDayIdx)
    {
		SkRRect rect = SkRRect::MakeRectXY(SkRect::MakeXYWH(startDayX, topY, dayWidth, botY-topY),r,r);
        skia_canvas->drawRRect(rect, paint);
    }
    else 
    {
		SkRRect startDay = SkRRect::MakeRectXY(SkRect::MakeXYWH(startDayX, topY, dayWidth, this->bounds.height()-topY),r,r);
        skia_canvas->drawRRect(startDay, paint);
        SkRRect coverDays = SkRRect::MakeRectXY(SkRect::MakeXYWH(endDayX, 0, dayWidth, botY),20,20);
        skia_canvas->drawRRect(coverDays, paint);
        SkRRect endDay = SkRRect::MakeRectXY(SkRect::MakeXYWH(startDayX+dayWidth, 0, dayWidth*(endDayIdx-startDayIdx-1), this->bounds.height()),r,r);
        skia_canvas->drawRRect(endDay, paint);
    }
    skia_canvas->restore();
}

bool TM_CalendarWeekView::PollEvents(TM_EventInput eventInput)
{
    eventInput.mousePressed = eventInput.mousePressed || eventInput.mouseHeld;
    if(this->bounds.contains(eventInput.mouseX,eventInput.mouseY))
    {
        if(eventInput.scrollY!=0)
		{
			this->scrollY+=eventInput.scrollY;
			SkScalar scrollLimitY = this->srcBounds.height()-this->bounds.height();
			this->scrollY = fmin(scrollLimitY, fmax(0, this->scrollY));
        }
        if(eventInput.mousePressed&&this->selected == false)
		{
			this->pressDayIndexStart = eventInput.mouseY-this->bounds.y()+this->scrollY;
            this->pressWeekIndexStart = eventInput.mouseX-this->bounds.x();
		}
        this->selected = eventInput.mousePressed;

		if(eventInput.mousePressed)
        {
			this->pressDayIndexEnd = eventInput.mouseY-this->bounds.y()+this->scrollY;
            this->pressWeekIndexEnd = eventInput.mouseX-this->bounds.x();
        }

		return true;
    }
    if(this->selected && !eventInput.mousePressed)
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

void TM_CalendarWeekView::setDaySpan(int daySpan)
{
    this->numDays = max(daySpan,0);
}

int TM_CalendarWeekView::getDaySpan()
{
    return this->numDays;
}

TM_CalendarWeekView::~TM_CalendarWeekView()
{

}