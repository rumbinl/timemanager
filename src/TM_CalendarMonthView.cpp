#include <TM_CalendarMonthView.hpp>

template <typename ContextType> TM_CalendarMonthView<ContextType>::TM_CalendarMonthView(SkRect bounds, ContextType* contextPtr, void (*setDateFunc)(ContextType* contextPtr, TM_YMD date), TM_YMD (*getDateFunc)(ContextType* contextPtr), TM_ViewSetting viewSetting) : TM_View(bounds, {})
{
    monthView = TM_MonthView<ContextType>(bounds, contextPtr, setDateFunc, getDateFunc);
    dataView = TM_TextView("", SkRect::MakeWH(0, 5));
    weekDayLabels = TM_HorizontalView(SkRect::MakeWH(0,10), {});
    previousMonth = TM_Button<TM_MonthView<ContextType>,int>("\ue314", SkRect::MakeWH(10,0), -1);
    nextMonth = TM_Button<TM_MonthView<ContextType>,int>("\ue315", SkRect::MakeWH(10,0), 1);
}

template <typename ContextType> void TM_CalendarMonthView<ContextType>::setDate(TM_YMD date)
{
    this->monthView.setDate(date);
}

#include <TM_CalendarView.hpp>
#include <TM_TaskManager.hpp>

template class TM_CalendarMonthView<TM_TaskManager>;
template class TM_CalendarMonthView<TM_CalendarView>;
