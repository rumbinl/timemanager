#include <TM_CalendarView.hpp>

TM_CalendarView::TM_CalendarView(SkRect bounds, TM_TaskManager* taskManPtr) : TM_View(bounds, {0.05,0.95},{}, {colorScheme[0],colorScheme[3],colorScheme[3],0,24,0,10})
{
    this->taskManPtr = taskManPtr;
    this->currentDate = getCurrentDate();
    this->weekView = new TM_CalendarWeekView(SkRect::MakeXYWH(0, 0, 0, 480), &this->currentDate, taskManPtr);
    this->monthView = new TM_CalendarMonthView(SkRect::MakeXYWH(0, 0, 250, 250), &this->currentDate);
   	this->buttonBar = new TM_HorizontalView(SkRect::MakeXYWH(0,0,0,40),{

			new TM_Button<TM_CalendarView>("Show Month", SkRect::MakeXYWH(0,0,0,40), 
				[](TM_CalendarView* context) 
				{
					context->getRenderObject(1)->setRenderObjectExistence(0,true);
					context->getRenderObject(0)->setRenderObjectExistence(0,false);
					context->getRenderObject(0)->setRenderObjectExistence(1,true);
				}, this),

			new TM_Button<TM_CalendarView>("Hide Month", SkRect::MakeXYWH(0,0,0,40), 
				[](TM_CalendarView* context) 
				{
					context->getRenderObject(1)->setRenderObjectExistence(0,false);
					context->getRenderObject(0)->setRenderObjectExistence(0,true);
					context->getRenderObject(0)->setRenderObjectExistence(1,false);
				}, this),

			new TM_Button<TM_CalendarWeekView>("Increase View Range", SkRect::MakeXYWH(0,0,0,40), 
				[](TM_CalendarWeekView* context)
				{
					context->setDaySpan(context->getDaySpan()+1);
				}, this->weekView),

    		new TM_Button<TM_CalendarWeekView>("Decrease View Range", SkRect::MakeXYWH(0,0,0,40),
				[](TM_CalendarWeekView* context)
				{
					context->setDaySpan(context->getDaySpan()-1);
				}, this->weekView)
		}
		, {colorScheme[0],colorScheme[3],colorScheme[3],0,24,0,0});

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
