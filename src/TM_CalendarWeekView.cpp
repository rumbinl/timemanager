#include <TM_UI.hpp>

TM_CalendarWeekView::TM_CalendarWeekView(SkRect bounds, std::chrono::year_month_day* focusDate, std::set<TM_Task>* tasks, int numDays, SkScalar hourHeight, TM_ViewSetting viewSetting) : TM_RenderObject(bounds, viewSetting)
{
	this->hourHeight = hourHeight;
	this->scrollY = 0;
    this->numDays = numDays;
    this->focusDate = focusDate;
    this->tasks = tasks;
}

void TM_CalendarWeekView::RenderTimes(SkCanvas* skia_canvas, SkFont* font)
{
    SkPaint paint;
	font->setSize(this->viewSetting.fontSize);
	SkFontMetrics fontMetrics;
	font->getMetrics(&fontMetrics);
	
	paint.setColor(this->viewSetting.borderColor);

	SkScalar yOff = -this->scrollY, y = 0.0f;
    SkScalar dayWidth = (this->bounds.width()-xOff)/(SkScalar)numDays;
    
	for(int i=0;i<24;i++)
	{
		std::string timeString = std::to_string(i)+":00";
		SkRect text_bounds;
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
	SkRect text_bounds;
	font->measureText("XX:XX", 5*sizeof(char), SkTextEncoding::kUTF8, &text_bounds, &paint);
	this->xOff = text_bounds.width();

	paint.setStyle(SkPaint::kStroke_Style);
	paint.setColor(this->viewSetting.borderColor);

	skia_canvas->drawRect(this->bounds, paint);

	paint.setStyle(SkPaint::kFill_Style);
	paint.setColor(this->viewSetting.backgroundColor);

	skia_canvas->drawRect(this->bounds, paint);

    font->setSize(this->viewSetting.fontSize);

    SkScalar dayWidth = (this->bounds.width() - this->xOff)/((SkScalar)this->numDays);
    SkScalar labelHeight = this->viewSetting.fontSize;
	this->yOff = labelHeight;

    for(int i=0;i<numDays;i++)
    {
        std::chrono::year_month_day currentDate = std::chrono::sys_days{*focusDate} + std::chrono::days{i};
        std::string date = dayNames[weekDayFromDate(currentDate)].substr(0,2)+" "+std::to_string(static_cast<unsigned>(currentDate.day()))+" "+monthNames[static_cast<unsigned>(currentDate.month())-1].substr(0,3);
        TM_TextView::Render(date, SkRect::MakeXYWH(this->bounds.x()+xOff+i*dayWidth,this->bounds.y(),dayWidth,labelHeight),skia_canvas,font);
    }

	skia_canvas->save();
	skia_canvas->clipRect(SkRect::MakeXYWH(this->bounds.x(),this->bounds.y()+labelHeight,this->bounds.width(),this->bounds.height()-labelHeight));
	skia_canvas->translate(this->bounds.x(), this->bounds.y()+labelHeight);

    RenderTimes(skia_canvas, font);

    paint.setColor(this->viewSetting.textColor);

	SkFontMetrics fontMetrics;
	font->getMetrics(&fontMetrics);

    for(int i=0;i<numDays;i++)
    {
        std::chrono::year_month_day currentDate = std::chrono::sys_days{*focusDate} + std::chrono::days{i};
        SkScalar y = -this->scrollY+this->viewSetting.padding;
        int c = 0;
        for(TM_Task task : *this->tasks) 
        {
            if(task.getDate()==currentDate)
            {
                SkScalar minutes = 30.0f;
                SkRect rect = SkRect::MakeXYWH(this->xOff + dayWidth*i, y, dayWidth, this->hourHeight/(60.0f/minutes));

                paint.setStyle(SkPaint::kFill_Style);
                paint.setColor(this->viewSetting.borderColor);
                skia_canvas->drawRect(rect, paint);
                paint.setColor(this->viewSetting.backgroundColor);
				SkScalar scaleFactor = 1;
				if(this->viewSetting.fontSize>rect.height())	
					scaleFactor = rect.height()/this->viewSetting.fontSize;
				font->setSize(rect.height());
                skia_canvas->drawString(task.getName().c_str(), this->xOff+dayWidth*i, y+rect.height()-scaleFactor*fontMetrics.fDescent, *font, paint);
                paint.setStyle(SkPaint::kStroke_Style);
                paint.setColor(this->viewSetting.backgroundColor);
                skia_canvas->drawRect(rect, paint);
                y+=this->hourHeight/(60.0f/minutes);
            }
        }
    }
    
    if(pressWeekIndexStart == -1) 
    { 
        skia_canvas->restore();
        return;
    }
    int startDayIdx=(int)floor((this->pressWeekIndexStart-this->xOff)/dayWidth),
        endDayIdx = (int)floor((this->pressWeekIndexEnd-this->xOff)/dayWidth);

    SkScalar firstY=fmax(0,pressDayIndexStart),secondY=fmax(0,pressDayIndexEnd);

    if(startDayIdx>endDayIdx) { std::swap(startDayIdx, endDayIdx); std::swap(firstY,secondY); }

    startDayIdx = min(max(startDayIdx,0),this->numDays);
    endDayIdx = min(max(endDayIdx,0),this->numDays);

    SkScalar startDayX = this->xOff+dayWidth*startDayIdx
            ,endDayX   = this->xOff+dayWidth*endDayIdx;

    SkScalar timeStep = this->hourHeight/4.0f;
    SkScalar topY = this->viewSetting.padding - this->scrollY + (timeStep)*round((firstY)/(timeStep)),
             botY = this->viewSetting.padding - this->scrollY + (timeStep)*round((secondY)/(timeStep));

    paint.setStyle(SkPaint::kFill_Style);
    paint.setColor(this->viewSetting.textColor);
    paint.setAlpha(100);

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
        SkRRect coverDays = SkRRect::MakeRectXY(SkRect::MakeXYWH(endDayX, 0, dayWidth, botY),r,r);
        skia_canvas->drawRRect(coverDays, paint);
        SkRRect endDay = SkRRect::MakeRectXY(SkRect::MakeXYWH(startDayX+dayWidth, 0, dayWidth*(endDayIdx-startDayIdx-1), this->bounds.height()),r,r);
        skia_canvas->drawRRect(endDay, paint);
    }
    
    skia_canvas->restore();
}

bool TM_CalendarWeekView::PollEvents(TM_EventInput eventInput)
{
    eventInput.mousePressed = eventInput.mousePressed || eventInput.mouseHeld;
	bool contains = this->bounds.contains(eventInput.mouseX,eventInput.mouseY);
    if(contains)
    {
        if(eventInput.scrollY!=0)
		{
			this->scrollY+=eventInput.scrollY;
			SkScalar scrollLimitY = this->srcBounds.height()-this->bounds.height()+this->yOff;
			this->scrollY = fmin(scrollLimitY, fmax(0, this->scrollY));
        }

        if(eventInput.mousePressed&&this->select == false)
		{
			this->pressDayIndexStart = eventInput.mouseY-this->bounds.y()-this->viewSetting.padding-this->yOff+this->scrollY;
            this->pressWeekIndexStart = eventInput.mouseX-this->bounds.x();
		}

        this->select = eventInput.mousePressed;

		if(eventInput.mousePressed)
        {
			this->pressDayIndexEnd = eventInput.mouseY-this->bounds.y()-this->viewSetting.padding-this->yOff+this->scrollY;
            this->pressWeekIndexEnd = eventInput.mouseX-this->bounds.x();
        }

		return true;
    }
    if(this->select && !eventInput.mousePressed)
	{
		this->select = false;
		this->pressDayIndexStart = this->pressDayIndexEnd = this->pressWeekIndexStart = this->pressWeekIndexEnd = -1;
		return true;
	}
    return false;
}

void TM_CalendarWeekView::setDaySpan(int daySpan)
{
    this->numDays = max(daySpan,1);
}

int TM_CalendarWeekView::getDaySpan()
{
    return this->numDays;
}

TM_CalendarWeekView::~TM_CalendarWeekView()
{

}
