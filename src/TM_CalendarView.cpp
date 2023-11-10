#include <TM_CalendarView.hpp>

TM_CalendarView::TM_CalendarView(SkRect bounds, TM_TaskManager* taskManPtr) : TM_View(bounds, {0.05,0.95},{}, {colorScheme[0],colorScheme[3],colorScheme[3],0,24,0,10})
{
    this->taskManPtr = taskManPtr;
    this->currentDate = getCurrentDate();
    this->weekView = new TM_CalendarWeekView(SkRect::MakeXYWH(0, 0, 0, 480), &this->currentDate, taskManPtr);
    this->monthView = new TM_CalendarMonthView<TM_CalendarView>(SkRect::MakeXYWH(0, 0, 250, 250), this, 
	[](TM_CalendarView* calendarView, TM_YMD date){
		calendarView->setReferenceDate(date);
	}, 
	[](TM_CalendarView* calendarView) -> TM_YMD
	{
		return calendarView->getReferenceDate();
	});
   	this->buttonBar = new TM_HorizontalView(SkRect::MakeXYWH(0,0,0,40),{

			new TM_Button<TM_CalendarView,int>("\ue8f4\uebcc", SkRect::MakeXYWH(0,0,0,40), 0, this, 
				[](TM_CalendarView* context,int data) 
				{
					context->getRenderObject(1)->setRenderObjectExistence(0,true);
					context->getRenderObject(0)->setRenderObjectExistence(0,false);
					context->getRenderObject(0)->setRenderObjectExistence(1,true);
				}, {colorScheme[1],colorScheme[2],colorScheme[3],0,24,5,5,true}),

			new TM_Button<TM_CalendarView,int>("\ue8f5\uebcc", SkRect::MakeXYWH(0,0,0,40), 0, this, 
				[](TM_CalendarView* context, int data) 
				{
					context->getRenderObject(1)->setRenderObjectExistence(0,false);
					context->getRenderObject(0)->setRenderObjectExistence(0,true);
					context->getRenderObject(0)->setRenderObjectExistence(1,false);
				}, {colorScheme[1],colorScheme[2],colorScheme[3],0,24,5,5,true}),

    		new TM_Button<TM_CalendarWeekView,int>("\ue3cb\ue8f3", SkRect::MakeXYWH(0,0,0,40), 0, this->weekView, 
				[](TM_CalendarWeekView* context, int data)
				{
					context->setDaySpan(context->getDaySpan()-1);
				}, {colorScheme[1],colorScheme[2],colorScheme[3],0,24,5,5,true}),

			new TM_Button<TM_CalendarWeekView,int>("\ue3cd\ue8f3", SkRect::MakeXYWH(0,0,0,40), 0, this->weekView, 
				[](TM_CalendarWeekView* context, int data)
				{
					context->setDaySpan(context->getDaySpan()+1);
				}, {colorScheme[1],colorScheme[2],colorScheme[3],0,24,5,5,true})
		});

	this->vitalView = new TM_View(SkRect::MakeEmpty(), {}, {
		this->monthView,
		this->weekView
	});

	this->addRenderObject(this->buttonBar);
    this->addRenderObject(this->vitalView);

	this->getRenderObject(1)->setRenderObjectExistence(0,true);
	this->getRenderObject(0)->setRenderObjectExistence(0,false);
	this->getRenderObject(0)->setRenderObjectExistence(1,true);
}

void TM_CalendarView::setReferenceDate(TM_YMD date)
{
	this->currentDate = date;
}

TM_YMD TM_CalendarView::getReferenceDate()
{
	return this->currentDate;
}
