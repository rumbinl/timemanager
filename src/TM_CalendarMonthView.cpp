#include <TM_CalendarMonthView.hpp>

TM_CalendarMonthView::TM_CalendarMonthView(SkRect bounds, void* contextPtr, void (*setDateFunc)(void* contextPtr, TM_YMD date), TM_YMD (*getDateFunc)(void* contextPtr), TM_ViewSetting viewSetting) : TM_View(bounds, {}, false, viewSetting)
{
    monthView = new TM_MonthView(SkRect::MakeEmpty(), contextPtr, setDateFunc, getDateFunc);
    saveMaxBounds = bounds;
    dataView = TM_Button<int>([](void* contextPtr) -> std::string {
        TM_CalendarMonthView* monthView = (TM_CalendarMonthView*)contextPtr;
        return monthNames[(unsigned)monthView->getMonthYear().month()-1]+" "+std::to_string((int)monthView->getMonthYear().year());
    }, SkRect::MakeWH(0, 5), 0, this, [](void* contextPtr, int data) {
        TM_CalendarMonthView* monthView = (TM_CalendarMonthView*)contextPtr;
        if(!monthView->getRenderObjectExistence(1))
        {
            monthView->setRenderObjectExistence(1,true);
            monthView->setMaxBounds(SkRect::MakeWH(monthView->getMaxBounds().width(), monthView->getSaveBounds().height()));
        }
        else
        {
            monthView->setRenderObjectExistence(1,false);
            monthView->setMaxBounds(SkRect::MakeWH(monthView->getMaxBounds().width(), TM_NormalHeight));
        }
        monthView->switchNavigators();
    });

    previousMonth = TM_Button<int>("\ue5e0", SkRect::MakeWH(60,0), -1, this->monthView, [](void* monthViewPtr, int data) {
        TM_MonthView* monthView = (TM_MonthView*)monthViewPtr;
        monthView->setMonthYear(monthView->getMonthYear()-std::chrono::months{1});
    }, {colorScheme[1],colorScheme[2],colorScheme[3],0,12,0,0,true});

    nextMonth = TM_Button<int>("\ue5e1", SkRect::MakeWH(60,0), 1, this->monthView, [](void* monthViewPtr, int data) {
        TM_MonthView* monthView = (TM_MonthView*)monthViewPtr;
        monthView->setMonthYear(monthView->getMonthYear()+std::chrono::months{1});
    }, {colorScheme[1],colorScheme[2],colorScheme[3],0,12,0,0,true});

    controlPanel = TM_HorizontalView(SkRect::MakeWH(TM_NormalWidth, TM_NormalHeight), {&previousMonth, &dataView, &nextMonth}, {0, 1.0, 0}, {colorScheme[0],colorScheme[2],colorScheme[3],0,12,0,0,false,20});

    this->addRenderObject(&this->controlPanel);
    this->addRenderObject(this->monthView);
    this->proportionTable = {0.0f, 1.0f};
    this->fit = true;
}

void TM_CalendarMonthView::switchNavigators()
{
    this->previousMonth.setExistence(!this->previousMonth.exists());
    this->nextMonth.setExistence(!this->nextMonth.exists());
}

std::chrono::year_month TM_CalendarMonthView::getMonthYear()
{
    return this->monthView->getMonthYear(); 
}

void TM_CalendarMonthView::setDate(TM_YMD date)
{
    this->monthView->setDate(date);
}

SkRect TM_CalendarMonthView::getSaveBounds()
{
    return this->saveMaxBounds;
}