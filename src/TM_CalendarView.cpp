#include <TM_UI.hpp>

TM_CalendarView::TM_CalendarView(SkRect bounds) : TM_View(bounds, {
    new TM_TextView("Month View", SkRect::MakeXYWH(0,0,0,40)), 
    new TM_TextView("Week View", SkRect::MakeXYWH(0,0,0,40)),
    new TM_TextView("Increase View Range", SkRect::MakeXYWH(0,0,0,40)),
    new TM_TextView("Decrease View Range", SkRect::MakeXYWH(0,0,0,40)),
})
{

}

bool TM_CalendarView::PollEvents(TM_EventInput eventInput)
{
    if(this->bounds.contains(eventInput.mouseX,eventInput.mouseY))
    {
    }
}

TM_CalendarView::~TM_CalendarView()
{

}