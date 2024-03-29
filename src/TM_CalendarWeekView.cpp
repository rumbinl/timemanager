#include <TM_CalendarWeekView.hpp>

TM_CalendarWeekView::TM_CalendarWeekView(SkRect bounds, std::chrono::year_month_day* focusDate, TM_TaskManager* taskManPtr, int numDays, SkScalar hourHeight, TM_ViewSetting viewSetting) : TM_RenderObject(bounds, viewSetting)
{
	this->hourHeight = hourHeight;
	this->scrollY = 0;
    this->numDays = numDays;
    this->focusDate = focusDate;
    this->taskManPtr = taskManPtr;
}

void TM_CalendarWeekView::RenderTimes(TM_RenderInfo renderInfo)
{
    SkPaint paint;
	renderInfo.textFont->setSize(this->viewSetting.fontSize*renderInfo.dpi);
	SkFontMetrics fontMetrics;
	renderInfo.textFont->getMetrics(&fontMetrics);
	
	paint.setColor(this->viewSetting.borderColor);

	SkScalar yOff = -this->scrollY, y = 0.0f;
    SkScalar dayWidth = (this->bounds.width()-xOff)/(SkScalar)numDays;

	this->hourHeight = std::fmin(this->bounds.height()/12.0f, 40.0f);
    
	for(int i=0;i<24;i++)
	{
		std::string timeString = std::to_string(i)+":00";
		SkRect text_bounds;
		renderInfo.textFont->measureText(timeString.c_str(), timeString.size()*sizeof(char), SkTextEncoding::kUTF8, &text_bounds, &paint);

		if(!i)
			y += viewSetting.fontSize+fontMetrics.fDescent;

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

bool TM_CalendarWeekView::DateRangeInView(TM_Task* task)
{
	return task->DateRangeInView(*this->focusDate, std::chrono::year_month_day{std::chrono::sys_days{*this->focusDate} + std::chrono::days{(unsigned)this->numDays - 1}});
}

void TM_CalendarWeekView::Render(TM_RenderInfo renderInfo)
{
    SkPaint paint;
	SkRect text_bounds;
    renderInfo.textFont->setSize(this->viewSetting.fontSize*renderInfo.dpi);
	renderInfo.textFont->measureText("XX:XX", 5*sizeof(char), SkTextEncoding::kUTF8, &text_bounds, &paint);
	this->xOff = text_bounds.width();

	paint.setStyle(SkPaint::kFill_Style);
	paint.setColor(this->viewSetting.backgroundColor);

	renderInfo.canvas->drawRect(this->bounds, paint);
	if(this->viewSetting.borderThickness>0)
	{
		paint.setStyle(SkPaint::kStroke_Style);
		paint.setColor(this->viewSetting.borderColor-1);
		renderInfo.canvas->drawRect(this->bounds, paint);
	}

	renderInfo.canvas->save();
	renderInfo.canvas->clipRRect(SkRRect::MakeRectXY(this->bounds, this->viewSetting.cornerRadius, this->viewSetting.cornerRadius));
	renderInfo.canvas->translate(this->bounds.x(), this->bounds.y());


    this->dayWidth = (this->bounds.width() - this->xOff)/((SkScalar)this->numDays);
    this->labelHeight = this->viewSetting.fontSize*renderInfo.dpi+2*this->viewSetting.paddingY;

	this->yOff = labelHeight;

    for(int i=0;i<numDays;i++)
    {
        std::chrono::year_month_day currentDate = std::chrono::sys_days{*focusDate} + std::chrono::days{i};
        std::string date = dayNames[weekDayFromDate(currentDate)].substr(0,2)+" "+std::to_string(static_cast<unsigned>(currentDate.day()))+" "+monthNames[static_cast<unsigned>(currentDate.month())-1].substr(0,3);
        TM_TextView::Render(date, SkRect::MakeXYWH(xOff+i*dayWidth,0,dayWidth,labelHeight),renderInfo, {colorScheme[1],colorScheme[2],colorScheme[3],0,12,0});
    }

	renderInfo.canvas->translate(0, labelHeight);
	renderInfo.canvas->clipRect(SkRect::MakeXYWH(0,0,this->bounds.width(),this->bounds.height()-labelHeight));

    RenderTimes(renderInfo);

	renderInfo.canvas->clipRect(SkRect::MakeXYWH(xOff,0,this->bounds.width(),this->bounds.height()-labelHeight));

    paint.setColor(this->viewSetting.textColor);

	SkFontMetrics fontMetrics;
	renderInfo.textFont->getMetrics(&fontMetrics);

	std::chrono::year_month_day currentDate;
	for(TM_TaskIt taskIt : this->taskManPtr->getTaskList()) 
	{
		TM_Task* task = *taskIt;
		SkColor color = this->viewSetting.borderColor;
		if(task->getSubtaskCount())
			color = SkColorSetA(color, 100);

		if(!task->getRepeat() && DateRangeInView(task))
		{
			RenderTask(task, task->getStartDate(), color, renderInfo);
		}
		else if(task->getRepeat())
		{
			TM_YMD lastDay = std::chrono::year_month_day{std::chrono::sys_days{*this->focusDate} + std::chrono::days{(unsigned)this->numDays - 1}},
			       startDate = task->RepeatFirstOccurence(*this->focusDate, lastDay), endDate = task->RepeatLastOccurence(*this->focusDate, lastDay);
			if(startDate != ZeroDate)
			{
				while(startDate <= endDate)
				{
					RenderTask(task, startDate, color, renderInfo);
					startDate = std::chrono::sys_days{startDate} + std::chrono::days{task->getRepeat()};
				}
			}
		}
	}
    
    if(this->select) 
    { 
		SkColor color = this->viewSetting.borderColor;
		RenderTask(&newTask, newTask.getStartDate(), color, renderInfo);
    }
    
    renderInfo.canvas->restore();
}

void TM_CalendarWeekView::RenderTask(TM_Task* task, TM_YMD startDate, SkColor color, TM_RenderInfo renderInfo)
{
	SkPaint paint;
	paint.setStyle(SkPaint::kFill_Style);
	std::chrono::days taskLength = std::chrono::sys_days{task->getEndDate()} - std::chrono::sys_days{task->getStartDate()};
	TM_Time startTime = task->getStartTime(),endTime = task->getEndTime(); 
	TM_YMD endDate = std::chrono::sys_days{startDate} + taskLength; 

	int startIndex = (std::chrono::sys_days{startDate}-std::chrono::sys_days{*this->focusDate}).count(),
		endIndex   = (std::chrono::sys_days{endDate}-std::chrono::sys_days{*this->focusDate}).count();

	SkScalar yOff = -this->scrollY+this->viewSetting.paddingY;
	SkScalar startDayX = this->xOff + std::fmax(dayWidth*startIndex,-dayWidth),
			 startDayY = yOff + this->hourHeight*((SkScalar)TM_TimeMinutes(startTime)/60.0f),
			 endDayX = this->xOff + dayWidth*endIndex,
			 endDayY = yOff + this->hourHeight*((SkScalar)TM_TimeMinutes(endTime)/60.0f);
	SkPoint points[2] = {{startDayX, std::fmin(startDayY,endDayY)},{endDayX+dayWidth,std::max(endDayY, startDayY)}};
	SkColor colors[2] = {task->getColor(), this->viewSetting.backgroundColor};
	
	if(task->getSubtaskCount())
		colors[0] = SkColorSetA(colors[0], 100);

	paint.setShader(SkGradientShader::MakeLinear(points, colors, NULL, 2, SkTileMode::kClamp));
	
	paint.setAntiAlias(true);
    if(startDate == endDate)
    {
		SkScalar minutes = std::fmax(TM_TimeMinutes(endTime) - TM_TimeMinutes(startTime), 15.0f);
		SkRect rect = SkRect::MakeXYWH(startDayX, startDayY, dayWidth, this->hourHeight*(minutes/60.0f));
        renderInfo.canvas->drawRRect(SkRRect::MakeRectXY(rect, 10, 10), paint);

		renderInfo.textFont->setSize(std::fmin(this->viewSetting.fontSize*renderInfo.dpi, this->hourHeight*(minutes/60.0f)));
    }
    else 
    {
		if(startIndex>=0)
		{
			SkVector corners[] = {{radius, radius}, {radius, radius}, {0, 0}, {0, 0}};
			SkRect startDay = SkRect::MakeXYWH(startDayX, startDayY, dayWidth, this->srcBounds.height()-startDayY);
			SkRRect startDayRRect;
			startDayRRect.setRectRadii(startDay, corners);
			renderInfo.canvas->drawRRect(startDayRRect, paint);
		}

        SkRect coverDays = SkRect::MakeXYWH(startDayX+dayWidth, 0, endDayX-startDayX-dayWidth, this->srcBounds.height());
        renderInfo.canvas->drawRect(coverDays, paint);

        SkRect endDay = SkRect::MakeXYWH(endDayX, 0, dayWidth, endDayY);
		SkVector corners[] = {{0, 0}, {0, 0}, {radius, radius}, {radius, radius}};
		SkRRect endDayRRect;
		endDayRRect.setRectRadii(endDay, corners);
        renderInfo.canvas->drawRRect(endDayRRect, paint);

		renderInfo.textFont->setSize(this->viewSetting.fontSize*renderInfo.dpi);
    }

	paint.setShader(nullptr);
	int r = (task->getColor()%(1<<24))>>16, g = (task->getColor()%(1<<16))>>8, b = task->getColor() % (1<<8);
	SkScalar luma = 0.2126f * (SkScalar)r + 0.7152f * (SkScalar)g + 0.0722f * (SkScalar)b;
	if(luma<40.0f)
		paint.setColor(SK_ColorWHITE);
	else
		paint.setColor(SK_ColorBLACK);

	SkFontMetrics fontMetrics;
	renderInfo.textFont->getMetrics(&fontMetrics);

	renderInfo.canvas->drawString(task->getName().c_str(), startDayX, startDayY+renderInfo.textFont->getSize()-fontMetrics.fDescent, *renderInfo.textFont, paint);

	paint.setStyle(SkPaint::kStroke_Style);
	paint.setColor(this->viewSetting.backgroundColor);
}

std::chrono::year_month_day TM_CalendarWeekView::getDateFromMouseX(TM_EventInput eventInput)
{
	int dayIdx = (int)floor((eventInput.mouseX-this->bounds.x()-this->xOff)/dayWidth);
	return std::chrono::sys_days{*this->focusDate}+std::chrono::days{dayIdx};
}

TM_Time TM_CalendarWeekView::getTimeFromMouseY(TM_EventInput eventInput)
{
	SkScalar timeStep = this->hourHeight/4.0f;
	SkScalar normY=fmax(0,eventInput.mouseY-this->bounds.y()-this->viewSetting.paddingY-this->yOff+this->scrollY);
	int minuteCount = (int)round((normY)/timeStep)*15;
	return {minuteCount/60,minuteCount%60};
}

bool TM_CalendarWeekView::PollTask(TM_Task* task, TM_YMD startDate, TM_EventInput eventInput)
{
	eventInput.mouseX -= this->bounds.x();
	eventInput.mouseY -= this->bounds.y() + this->yOff;

	std::chrono::days taskLength = std::chrono::sys_days{task->getEndDate()} - std::chrono::sys_days{task->getStartDate()};
	TM_Time startTime = task->getStartTime(),endTime = task->getEndTime(); 
	TM_YMD endDate = std::chrono::sys_days{startDate} + taskLength; 

	int startIndex = (std::chrono::sys_days{startDate}-std::chrono::sys_days{*this->focusDate}).count(),
		endIndex   = (std::chrono::sys_days{endDate}-std::chrono::sys_days{*this->focusDate}).count();

	SkScalar yOff = -this->scrollY+this->viewSetting.paddingY;
	SkScalar startDayX = this->xOff + dayWidth*startIndex,
			 startDayY = yOff + this->hourHeight*((SkScalar)TM_TimeMinutes(startTime)/60.0f),
			 endDayX = this->xOff + dayWidth*endIndex,
			 endDayY = yOff + this->hourHeight*((SkScalar)TM_TimeMinutes(endTime)/60.0f);
	
    if(task->getStartDate() == task->getEndDate())
    {
		SkScalar minutes = std::fmax(TM_TimeMinutes(endTime) - TM_TimeMinutes(startTime), 15.0f);
		SkRect rect = SkRect::MakeXYWH(startDayX, startDayY, dayWidth, this->hourHeight*(minutes/60.0f));
		return rect.contains(eventInput.mouseX, eventInput.mouseY);
    }
    else 
    {
		SkRect startDay = SkRect::MakeXYWH(startDayX, startDayY, dayWidth, this->srcBounds.height()-startDayY);

        SkRect coverDays = SkRect::MakeXYWH(startDayX+dayWidth, 0, endDayX-startDayX-dayWidth, this->srcBounds.height());

        SkRect endDay = SkRect::MakeXYWH(endDayX, 0, dayWidth, endDayY);
		return startDay.contains(eventInput.mouseX, eventInput.mouseY) || 
			   coverDays.contains(eventInput.mouseX, eventInput.mouseY) || 
			   endDay.contains(eventInput.mouseX, eventInput.mouseY);
    }
	return false;
}

bool TM_CalendarWeekView::PollEvents(TM_EventInput eventInput)
{
	bool contains = this->bounds.contains(eventInput.mouseX,eventInput.mouseY), shouldUpdate = false;
	if(contains) 
	{
		if(eventInput.scrollY!=0)
		{
			this->scrollY+=eventInput.scrollY;
			SkScalar scrollLimitY = fmax(0,this->srcBounds.height()-this->bounds.height()+this->yOff);
			this->scrollY = fmin(scrollLimitY, fmax(0, this->scrollY));
		}

		if(eventInput.mouseHeld)
		{
			if(!this->select)
			{
				this->newTask.setStartDate(this->getDateFromMouseX(eventInput));
				this->newTask.setStartTime(this->getTimeFromMouseY(eventInput));
				this->select = true;
			}

			this->newTask.setEndDate(this->getDateFromMouseX(eventInput));
			this->newTask.setEndTime(this->getTimeFromMouseY(eventInput));
		}

		if(eventInput.mousePressed)
		{
			std::multiset<TM_TaskIt,TM_Task::TM_TaskItCompare>::iterator taskIt = this->taskManPtr->getTaskList().end(); 
			while(taskIt != this->taskManPtr->getTaskList().begin())
			{
				taskIt--;
				if(!(**taskIt)->getRepeat() && DateRangeInView(**taskIt))
				{
					if(this->PollTask(**taskIt, (**taskIt)->getStartDate(), eventInput))
					{
						this->taskManPtr->setCurrentTask(taskIt);
						this->select = false;
						break;
					}
				}
				else if((**taskIt)->getRepeat())
				{
					TM_YMD lastDay = std::chrono::year_month_day{std::chrono::sys_days{*this->focusDate} + std::chrono::days{(unsigned)this->numDays - 1}},
					       startDate = (**taskIt)->RepeatFirstOccurence(*this->focusDate, lastDay), endDate = (**taskIt)->RepeatLastOccurence(*this->focusDate, lastDay);
					if(startDate != ZeroDate)
					{
						while(startDate <= endDate)
						{
							if(this->PollTask(**taskIt, startDate, eventInput))
							{
								this->taskManPtr->setCurrentTask(taskIt);
								this->select = false;
								break;
							}
							startDate = std::chrono::sys_days{startDate} + std::chrono::days{(**taskIt)->getRepeat()};
						}
					}
				}
			}
		}
	}

    if(this->select && !eventInput.mouseHeld)
	{
		TM_Task* task = new TM_Task(this->newTask);
		task->setName("New Event");
		this->taskManPtr->addTask(task);
		this->select = false;
	}
	return contains;
}

void TM_CalendarWeekView::setDaySpan(int daySpan)
{
    this->numDays = std::max(daySpan,1);
}

int TM_CalendarWeekView::getDaySpan()
{
    return this->numDays;
}

std::chrono::year_month_day* TM_CalendarWeekView::getCurrentDatePtr()
{
	return this->focusDate;
}
