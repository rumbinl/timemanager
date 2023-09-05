#include <TM_UI.hpp>

TM_CalendarView::TM_CalendarView(SkRect bounds) : TM_View(bounds, {})
{
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

    this->renderObjects.push_back(new TM_TextView("Increase View Range", SkRect::MakeXYWH(0,0,0,40)));
    this->renderObjects.push_back(new TM_TextView("Decrease View Range", SkRect::MakeXYWH(0,0,0,40)));
    this->renderObjects.push_back(new TM_CalendarMonthView(SkRect::MakeXYWH(0, 0, 0, 480), 9, 2023));
    this->renderObjects.push_back(new TM_CalendarWeekView(SkRect::MakeXYWH(0, 0, 0, 480), 7));
}

TM_CalendarView::~TM_CalendarView()
{

}