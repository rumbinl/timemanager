#include <TM_CalendarView.hpp>

TM_CalendarView::TM_CalendarView(SkRect bounds, std::set<TM_Task>* tasks) : TM_View(bounds, {}, {colorScheme[0],colorScheme[3],colorScheme[3],0,24,0,10})
{
    this->tasks = tasks;
    this->currentDate = getCurrentDate();
    this->weekView = new TM_CalendarWeekView(SkRect::MakeXYWH(0, 0, 0, 480), &this->currentDate, tasks);
    this->monthView = new TM_CalendarMonthView<TM_CalendarView>(SkRect::MakeXYWH(0, 0, 0, 480), [](TM_CalendarView* context, std::chrono::year_month_day date)
    {
        context->currentDate = date;
    },this);
   	this->buttonBar = new TM_HorizontalView(SkRect::MakeXYWH(0,0,0,40),{

			new TM_Button<TM_CalendarView>("Show Month", SkRect::MakeXYWH(0,0,0,40), 
				[](TM_CalendarView* context) 
				{
					context->setRenderObjectExistence(1,true);
					context->getBarView()->setRenderObjectExistence(1,true);
					context->getBarView()->setRenderObjectExistence(0,false);
				}, this),

			new TM_Button<TM_CalendarView>("Hide Month", SkRect::MakeXYWH(0,0,0,40), 
				[](TM_CalendarView* context) 
				{
					context->setRenderObjectExistence(1,false);
					context->getBarView()->setRenderObjectExistence(0,true);
					context->getBarView()->setRenderObjectExistence(1,false);
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
		, true, {colorScheme[0],colorScheme[3],colorScheme[3],0,24,0,0});
	this->addRenderObject(this->buttonBar);
    this->addRenderObject(this->monthView);
    this->addRenderObject(this->weekView);

    this->setRenderObjectExistence(1,true);
	this->getBarView()->setRenderObjectExistence(0,false);
	this->getBarView()->setRenderObjectExistence(1,true);
}

TM_HorizontalView* TM_CalendarView::getBarView()
{
	return this->buttonBar;
}
