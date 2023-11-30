#include <TM_CalendarView.hpp>

TM_CalendarView::TM_CalendarView(SkRect bounds, TM_TaskManager* taskManPtr) : TM_View(bounds, {1.0},{}, {colorScheme[0],colorScheme[3],colorScheme[3],0,24,0,10})
{
    this->taskManPtr = taskManPtr;
    this->currentDate = getCurrentDate();
    this->weekView = new TM_CalendarWeekView(SkRect::MakeWH(0, 480), &this->currentDate, taskManPtr);
    this->monthView = new TM_CalendarMonthView(SkRect::MakeWH(0, 250), (void*)this, 
		[](void* calendarView, TM_YMD date){

			((TM_CalendarView*)calendarView)->setReferenceDate(date);
		}, 
		[](void* calendarView) -> TM_YMD
		{
			return ((TM_CalendarView*)calendarView)->getReferenceDate();
		}
	);

	this->vitalView = new TM_View(SkRect::MakeEmpty(), {}, {
		this->monthView,
		new TM_View(SkRect::MakeEmpty(), {0.10, 0.90}, { 
			new TM_HorizontalView(SkRect::MakeWH(200,0), { 
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
			}, {}, {colorScheme[0], colorScheme[2], colorScheme[3], 0, 24, 0, 0, false, 20}),
			this->weekView
		})
	});

    this->addRenderObject(this->vitalView);
}

void TM_CalendarView::setReferenceDate(TM_YMD date)
{
	this->currentDate = date;
}

TM_YMD TM_CalendarView::getReferenceDate()
{
	return this->currentDate;
}
