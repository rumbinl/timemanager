#include <TM_UI.hpp>

TM_CalendarWeekView::TM_CalendarWeekView(SkRect bounds, std::chrono::year_month_day* focusDate, std::multiset<TM_Task*, TM_TaskPtrCompare>* tasksPtr, int numDays, SkScalar hourHeight, TM_ViewSetting viewSetting) : TM_RenderObject(bounds, viewSetting)
{
	this->hourHeight = hourHeight;
	this->scrollY = 0;
    this->numDays = numDays;
    this->focusDate = focusDate;
    this->tasks = tasksPtr;
}

void TM_CalendarWeekView::RenderTimes(TM_RenderInfo renderInfo)
{
    SkPaint paint;
	renderInfo.textFont->setSize(this->viewSetting.fontSize);
	SkFontMetrics fontMetrics;
	renderInfo.textFont->getMetrics(&fontMetrics);
	
	paint.setColor(this->viewSetting.borderColor);

	SkScalar yOff = -this->scrollY, y = 0.0f;
    SkScalar dayWidth = (this->bounds.width()-xOff)/(SkScalar)numDays;

	this->hourHeight = this->bounds.height()/12.0f;
    
	for(int i=0;i<24;i++)
	{
		std::string timeString = std::to_string(i)+":00";
		SkRect text_bounds;
		renderInfo.textFont->measureText(timeString.c_str(), timeString.size()*sizeof(char), SkTextEncoding::kUTF8, &text_bounds, &paint);

		if(!i)
		{
			this->viewSetting.paddingY = text_bounds.height()-fontMetrics.fBottom;
			y += this->viewSetting.paddingY;
		}

		renderInfo.canvas->drawString(timeString.c_str(), xOff-text_bounds.width(), y+yOff+fontMetrics.fBottom, *renderInfo.textFont, paint);
		renderInfo.canvas->drawLine(xOff, y+yOff, this->bounds.width(), y+yOff, paint);
		y += this->hourHeight;
	}
	renderInfo.canvas->drawLine(xOff, y+yOff, this->bounds.width(), y+yOff, paint);
	this->srcBounds.setWH(this->srcBounds.width(), y);
    for(int i=1;i<numDays;i++)
    {
        SkScalar x = xOff + i*dayWidth;
        renderInfo.canvas->drawLine(x, 0, x, this->srcBounds.height(),paint);
    }
}

void TM_CalendarWeekView::Render(TM_RenderInfo renderInfo)
{
    SkPaint paint;
	SkRect text_bounds;
	renderInfo.textFont->measureText("XX:XX", 5*sizeof(char), SkTextEncoding::kUTF8, &text_bounds, &paint);
	this->xOff = text_bounds.width();

	paint.setStyle(SkPaint::kFill_Style);
	paint.setColor(this->viewSetting.backgroundColor);

	renderInfo.canvas->drawRect(this->bounds, paint);

	paint.setStyle(SkPaint::kStroke_Style);
	paint.setColor(this->viewSetting.borderColor);

	renderInfo.canvas->drawRect(this->bounds, paint);

    renderInfo.textFont->setSize(this->viewSetting.fontSize);

    this->dayWidth = (this->bounds.width() - this->xOff)/((SkScalar)this->numDays);
    this->labelHeight = this->viewSetting.fontSize+2*this->viewSetting.paddingY;

	this->yOff = labelHeight;

    for(int i=0;i<numDays;i++)
    {
        std::chrono::year_month_day currentDate = std::chrono::sys_days{*focusDate} + std::chrono::days{i};
        std::string date = dayNames[weekDayFromDate(currentDate)].substr(0,2)+" "+std::to_string(static_cast<unsigned>(currentDate.day()))+" "+monthNames[static_cast<unsigned>(currentDate.month())-1].substr(0,3);
        TM_TextView::Render(date, SkRect::MakeXYWH(this->bounds.x()+xOff+i*dayWidth,this->bounds.y(),dayWidth,labelHeight),renderInfo, {colorScheme[1],colorScheme[2],colorScheme[3],0,24,0});
    }

	renderInfo.canvas->save();
	renderInfo.canvas->clipRect(SkRect::MakeXYWH(this->bounds.x(),this->bounds.y()+labelHeight,this->bounds.width(),this->bounds.height()-labelHeight));
	renderInfo.canvas->translate(this->bounds.x(), this->bounds.y()+labelHeight);

    RenderTimes(renderInfo);

    paint.setColor(this->viewSetting.textColor);

	SkFontMetrics fontMetrics;
	renderInfo.textFont->getMetrics(&fontMetrics);

	std::chrono::year_month_day currentDate;
	for(TM_Task* task : *this->tasks) 
	{
		if(task->getStartDate()>=*this->focusDate && task->getStartDate() < std::chrono::sys_days{*this->focusDate} + std::chrono::days{this->numDays})
			this->RenderTask(task, this->viewSetting.borderColor, renderInfo);
	}
    
    if(this->select) 
    { 
		// Render selection
		SkColor color = this->viewSetting.borderColor;
		color = SkColorSetA(color, 100);
		RenderTask(&newTask, color, renderInfo);
    }
    
    renderInfo.canvas->restore();
}

void TM_CalendarWeekView::RenderTask(TM_Task* task, SkColor color, TM_RenderInfo renderInfo)
{
	SkPaint paint;
	paint.setStyle(SkPaint::kFill_Style);
	paint.setColor(color);

	int startIndex = (std::chrono::sys_days{task->getStartDate()}-std::chrono::sys_days{*this->focusDate}).count(),
		endIndex   = (std::chrono::sys_days{task->getEndDate  ()}-std::chrono::sys_days{*this->focusDate}).count();

	SkScalar yOff = -this->scrollY+this->viewSetting.paddingY;
	SkScalar startDayX = this->xOff + dayWidth*startIndex,
			 startDayY = yOff + this->hourHeight*((SkScalar)TM_TimeMinutes(task->getStartTime())/60.0f),
			 endDayX = this->xOff + dayWidth*endIndex,
			 endDayY = yOff + this->hourHeight*((SkScalar)TM_TimeMinutes(task->getEndTime())/60.0f);
	
    if(task->getStartDate() == task->getEndDate())
    {
		SkScalar minutes = TM_TimeMinutes(task->getEndTime()) - TM_TimeMinutes(task->getStartTime());
		SkRect rect = SkRect::MakeXYWH(startDayX, startDayY, dayWidth, this->hourHeight*(minutes/60.0f));
        renderInfo.canvas->drawRect(rect, paint);
    }
    else 
    {
		SkRect startDay = SkRect::MakeXYWH(startDayX, startDayY, dayWidth, this->srcBounds.height()-startDayY);
        renderInfo.canvas->drawRect(startDay, paint);

        SkRect coverDays = SkRect::MakeXYWH(startDayX+dayWidth, 0, endDayX-startDayX-dayWidth, this->srcBounds.height());
        renderInfo.canvas->drawRect(coverDays, paint);

        SkRect endDay = SkRect::MakeXYWH(endDayX, 0, dayWidth, endDayY);
        renderInfo.canvas->drawRect(endDay, paint);
    }

	paint.setColor(this->viewSetting.backgroundColor);
	renderInfo.textFont->setSize(this->viewSetting.fontSize);

	renderInfo.canvas->drawString(task->getName().c_str(), startDayX, startDayY+this->viewSetting.fontSize, *renderInfo.textFont, paint);

	paint.setStyle(SkPaint::kStroke_Style);
	paint.setColor(this->viewSetting.backgroundColor);
}

bool TM_CalendarWeekView::PollEvents(TM_EventInput eventInput)
{
    eventInput.mousePressed = eventInput.mousePressed || eventInput.mouseHeld;
	bool contains = this->bounds.contains(eventInput.mouseX,eventInput.mouseY), shouldUpdate = false;
	if(contains && eventInput.scrollY!=0)
	{
		this->scrollY+=eventInput.scrollY;
		SkScalar scrollLimitY = fmax(0,this->srcBounds.height()-this->bounds.height()+this->yOff);
		this->scrollY = fmin(scrollLimitY, fmax(0, this->scrollY));
	}

    if(contains && eventInput.mouseHeld)
    {
		SkScalar timeStep = this->hourHeight/4.0f;

        if(!this->select)
		{
			int startDayIdx = (int)floor((eventInput.mouseX-this->bounds.x()-this->xOff)/dayWidth);
			std::chrono::year_month_day startDate = std::chrono::sys_days{*this->focusDate}+std::chrono::days{startDayIdx};
			this->newTask.setStartDate(startDate);

			SkScalar firstY=fmax(0,eventInput.mouseY-this->bounds.y()-this->viewSetting.paddingY-this->yOff+this->scrollY);

			int startMin = (int)round((firstY)/timeStep)*15;
			this->newTask.setStartTime({startMin/60,startMin%60});
			this->select = true;
		}

		int endDayIdx = (int)floor((eventInput.mouseX-this->bounds.x()-this->xOff)/dayWidth);
		std::chrono::year_month_day endDate = std::chrono::sys_days{*this->focusDate}+std::chrono::days{endDayIdx};
		this->newTask.setEndDate(endDate);

		SkScalar secondY=fmax(0,eventInput.mouseY-this->bounds.y()-this->viewSetting.paddingY-this->yOff+this->scrollY);
		int endMin = (int)round((secondY)/timeStep)*15;
		this->newTask.setEndTime({endMin/60,endMin%60});
    }

    if(this->select && !eventInput.mouseHeld)
	{
		TM_Task* task = new TM_Task(this->newTask);
		task->setName("New Task");
		this->tasks->insert(task);
		this->select = false;
	}
	return contains;
}

void TM_CalendarWeekView::setDaySpan(int daySpan)
{
    this->numDays = max(daySpan,1);
}

int TM_CalendarWeekView::getDaySpan()
{
    return this->numDays;
}
