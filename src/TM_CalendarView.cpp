#include <TM_CalendarView.hpp>

TM_CalendarView::TM_CalendarView(SkRect bounds, TM_TaskManager* taskManPtr) : TM_View(bounds,{},{}, {colorScheme[0],colorScheme[3],colorScheme[3],0,24,10,10,false,0})
{
    this->taskManPtr = taskManPtr;
    this->currentDate = getCurrentDate();
    this->weekView = new TM_CalendarWeekView(SkRect::MakeEmpty(), &this->currentDate, taskManPtr);
    this->monthView = new TM_CalendarMonthView(SkRect::MakeEmpty(), (void*)this, 
		[](void* calendarView, TM_YMD date){

			((TM_CalendarView*)calendarView)->setReferenceDate(date);
		}, 
		[](void* calendarView) -> TM_YMD
		{
			return ((TM_CalendarView*)calendarView)->getReferenceDate();
		}
	);

	this->fit = true;

	this->addRenderObject(this->monthView);
	this->addRenderObject(
		new TM_View(SkRect::MakeEmpty(), {0, 1.0}, { 
			new TM_HorizontalView(SkRect::MakeWH(0,50), { 
				new TM_Button<int>("\ue3cb\ue8f3", SkRect::MakeEmpty(), 0, this->weekView, 
					[](void* contextPtr, int data)
					{
						TM_CalendarWeekView* context = (TM_CalendarWeekView*)contextPtr;
						context->setDaySpan(context->getDaySpan()-1);
					}, {colorScheme[1],colorScheme[2],colorScheme[3],0,24,5,5,true}),

				new TM_Button<int>("\ue3cd\ue8f3", SkRect::MakeEmpty(), 0, this->weekView, 
					[](void* contextPtr, int data)
					{
						TM_CalendarWeekView* context = (TM_CalendarWeekView*)contextPtr;
						context->setDaySpan(context->getDaySpan()+1);
					}, {colorScheme[1],colorScheme[2],colorScheme[3],0,24,5,5,true})
			}),
			this->weekView
		}, {colorScheme[0], colorScheme[2], colorScheme[3], 0, 24, 0, 0, false, 20}));
}

void TM_CalendarView::setReferenceDate(TM_YMD date)
{
	this->currentDate = date;
}

TM_YMD TM_CalendarView::getReferenceDate()
{
	return this->currentDate;
}
