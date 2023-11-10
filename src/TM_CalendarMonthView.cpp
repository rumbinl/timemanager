#include <TM_CalendarMonthView.hpp>


template <typename ContextType> TM_CalendarMonthView<ContextType>::TM_CalendarMonthView(SkRect bounds, ContextType* contextPtr, void (*setDateFunc)(ContextType* contextPtr, TM_YMD date), TM_YMD (*getDateFunc)(ContextType* contextPtr), TM_ViewSetting viewSetting) : TM_View(bounds, {}, viewSetting)
{
    monthView = new TM_MonthView<ContextType>(SkRect::MakeWH(50,50), contextPtr, setDateFunc, getDateFunc);
    dataView = TM_TextView("", SkRect::MakeWH(0, 5));
    weekDayLabels = TM_HorizontalView(SkRect::MakeEmpty(), {});
    previousMonth = TM_Button<TM_MonthView<ContextType>,int>("\ue314", SkRect::MakeWH(10,0), -1, this->monthView, [](TM_MonthView<ContextType>* monthView, int data) {
        monthView->setMonthYear(monthView->getMonthYear()-std::chrono::months{1});
    }, {colorScheme[1],colorScheme[2],colorScheme[3],0,24,0,0,true});
    nextMonth = TM_Button<TM_MonthView<ContextType>,int>("\ue315", SkRect::MakeWH(10,0), 1, this->monthView, [](TM_MonthView<ContextType>* monthView, int data) {
        monthView->setMonthYear(monthView->getMonthYear()+std::chrono::months{1});
    }, {colorScheme[1],colorScheme[2],colorScheme[3],0,24,0,0,true});
    controlPanel = TM_HorizontalView(SkRect::MakeEmpty(), {&previousMonth, &dataView, &nextMonth}, {0.1, 0.8, 0.1});

    for(std::string weekName : dayNames)
        weekDayLabels.addRenderObject(new TM_TextView(weekName.substr(0,2), SkRect::MakeEmpty(), viewSetting={colorScheme[0],colorScheme[2],colorScheme[3],0,24,0,0}));

    this->addRenderObject(&this->controlPanel);
    this->addRenderObject(&this->weekDayLabels);
    this->addRenderObject(this->monthView);
    this->proportionTable = {0.05,0.05,0.90};
    this->fit = true;
}

template <typename ContextType> void TM_CalendarMonthView<ContextType>::Render(TM_RenderInfo renderInfo)
{
    this->dataView.setText(monthNames[(unsigned)this->monthView->getMonthYear().month()-1]+" "+std::to_string((int)this->monthView->getMonthYear().year()));
    TM_View::Render(renderInfo);
} 

template <typename ContextType> void TM_CalendarMonthView<ContextType>::setDate(TM_YMD date)
{
    this->monthView->setDate(date);
}

#include <TM_CalendarView.hpp>
#include <TM_TaskManager.hpp>

template class TM_CalendarMonthView<TM_TaskManager>;
template class TM_CalendarMonthView<TM_CalendarView>;