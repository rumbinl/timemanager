#include <TM_CalendarMonthView.hpp>

TM_CalendarMonthView::TM_CalendarMonthView(SkRect bounds, void* contextPtr, void (*setDateFunc)(void* contextPtr, TM_YMD date), TM_YMD (*getDateFunc)(void* contextPtr), TM_ViewSetting viewSetting) : TM_View(bounds, {}, false, viewSetting)
{
    monthView = new TM_MonthView(SkRect::MakeWH(50,50), contextPtr, setDateFunc, getDateFunc);
    dataView = TM_TextView("", SkRect::MakeWH(0, 5));
    weekDayLabels = TM_HorizontalView(SkRect::MakeEmpty(), {});
    previousMonth = TM_Button<int>("\ue5e0", SkRect::MakeWH(10,0), -1, this->monthView, [](void* monthViewPtr, int data, TM_Button<int>* self) {
        TM_MonthView* monthView = (TM_MonthView*)monthViewPtr;
        monthView->setMonthYear(monthView->getMonthYear()-std::chrono::months{1});
    }, {colorScheme[1],colorScheme[2],colorScheme[3],0,24,0,0,true});
    nextMonth = TM_Button<int>("\ue5e1", SkRect::MakeWH(10,0), 1, this->monthView, [](void* monthViewPtr, int data, TM_Button<int>* self) {
        TM_MonthView* monthView = (TM_MonthView*)monthViewPtr;
        monthView->setMonthYear(monthView->getMonthYear()+std::chrono::months{1});
    }, {colorScheme[1],colorScheme[2],colorScheme[3],0,24,0,0,true});
    controlPanel = TM_HorizontalView(SkRect::MakeEmpty(), {&previousMonth, &dataView, &nextMonth}, {0.1, 0.8, 0.1});

    for(std::string weekName : dayNames)
        weekDayLabels.addRenderObject(new TM_TextView(weekName.substr(0,2), SkRect::MakeEmpty(), viewSetting={colorScheme[0],colorScheme[2],colorScheme[3],0,24,0,0}));

    this->addRenderObject(&this->controlPanel);
    this->addRenderObject(&this->weekDayLabels);
    this->addRenderObject(this->monthView);
    this->proportionTable = {0.15,0.05,0.80};
    this->fit = true;
}

void TM_CalendarMonthView::Render(TM_RenderInfo renderInfo)
{
    this->dataView.setText(monthNames[(unsigned)this->monthView->getMonthYear().month()-1]+" "+std::to_string((int)this->monthView->getMonthYear().year()));
    TM_View::Render(renderInfo);
} 

void TM_CalendarMonthView::setDate(TM_YMD date)
{
    this->monthView->setDate(date);
}