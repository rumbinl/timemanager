#include <TM_CalendarMonthView.hpp>

TM_CalendarMonthView::TM_CalendarMonthView(SkRect bounds, void* contextPtr, void (*setDateFunc)(void* contextPtr, TM_YMD date), TM_YMD (*getDateFunc)(void* contextPtr), TM_ViewSetting viewSetting) : TM_View(bounds, {}, false, viewSetting)
{
    monthView = new TM_MonthView(SkRect::MakeWH(0,50), contextPtr, setDateFunc, getDateFunc);
    dataView = TM_TextView("", SkRect::MakeWH(0, 5));
    previousMonth = TM_Button<int>("\ue5e0", SkRect::MakeWH(10,0), -1, this->monthView, [](void* monthViewPtr, int data) {
        TM_MonthView* monthView = (TM_MonthView*)monthViewPtr;
        monthView->setMonthYear(monthView->getMonthYear()-std::chrono::months{1});
    }, {colorScheme[1],colorScheme[2],colorScheme[3],0,24,0,0,true});
    nextMonth = TM_Button<int>("\ue5e1", SkRect::MakeWH(10,0), 1, this->monthView, [](void* monthViewPtr, int data) {
        TM_MonthView* monthView = (TM_MonthView*)monthViewPtr;
        monthView->setMonthYear(monthView->getMonthYear()+std::chrono::months{1});
    }, {colorScheme[1],colorScheme[2],colorScheme[3],0,24,0,0,true});
    controlPanel = TM_HorizontalView(SkRect::MakeWH(TM_NormalWidth, 50), {&previousMonth, &dataView, &nextMonth}, {0.1, 0.8, 0.1}, {colorScheme[0],colorScheme[2],colorScheme[3],0,24,0,0,false,20});

    this->addRenderObject(&this->controlPanel);
    this->addRenderObject(this->monthView);
    this->proportionTable = {};
    this->fit = false;
}

void TM_CalendarMonthView::Render(TM_RenderInfo renderInfo)
{
    this->dataView.setText(monthNames[(unsigned)this->monthView->getMonthYear().month()-1]+" "+std::to_string((int)this->monthView->getMonthYear().year()));
    this->controlPanel.setBounds(SkRect::MakeWH(TM_NormalWidth, 50));
    this->monthView->setBounds(SkRect::MakeWH(0,this->bounds.height()-50));
    TM_View::Render(renderInfo);
} 

void TM_CalendarMonthView::setDate(TM_YMD date)
{
    this->monthView->setDate(date);
}