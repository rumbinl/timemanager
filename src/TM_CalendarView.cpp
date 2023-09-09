#include <TM_CalendarView.hpp>

TM_CalendarView::TM_CalendarView(SkRect bounds, std::set<TM_Task>* tasks) : TM_View(bounds, {})
{
    this->tasks = tasks;
    this->currentDate = getCurrentDate();
    this->weekView = new TM_CalendarWeekView(SkRect::MakeXYWH(0, 0, 0, 480), &this->currentDate, tasks);
    this->monthView = new TM_CalendarMonthView<TM_CalendarView>(SkRect::MakeXYWH(0, 0, 0, 480), [](TM_CalendarView* context, std::chrono::year_month_day date)
    {
        context->currentDate = date;
    },this);
    this->renderObjects.push_back(new TM_Button<TM_CalendarView>("Month View", SkRect::MakeXYWH(0,0,0,40), 

        [](TM_CalendarView* context) 
        {
            context->setRenderObjectExistence(2,false);
            context->setRenderObjectExistence(3,false);
            context->setRenderObjectExistence(4,true);
            context->setRenderObjectExistence(5,false);
        }, this)); 

    this->renderObjects.push_back(new TM_Button<TM_CalendarView>("Week View", SkRect::MakeXYWH(0,0,0,40), 
        [](TM_CalendarView* context) 
        {
            context->setRenderObjectExistence(2,true);
            context->setRenderObjectExistence(3,true);
            context->setRenderObjectExistence(4,false);
            context->setRenderObjectExistence(5,true);
        }, this));

    this->renderObjects.push_back(new TM_Button<TM_CalendarWeekView>("Increase View Range", SkRect::MakeXYWH(0,0,0,40), 
        [](TM_CalendarWeekView* context)
        {
            context->setDaySpan(context->getDaySpan()+1);
        }
    , this->weekView));
    this->renderObjects.push_back(new TM_Button<TM_CalendarWeekView>("Decrease View Range", SkRect::MakeXYWH(0,0,0,40),
        [](TM_CalendarWeekView* context)
        {
            context->setDaySpan(context->getDaySpan()-1);
        }, this->weekView
    ));
    this->renderObjects.push_back(this->monthView);
    this->renderObjects.push_back(this->weekView);

    this->setRenderObjectExistence(2,true);
    this->setRenderObjectExistence(3,true);
    this->setRenderObjectExistence(4,false);
    this->setRenderObjectExistence(5,true);
}

TM_CalendarView::~TM_CalendarView()
{

}
