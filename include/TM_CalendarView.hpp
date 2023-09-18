#pragma once

#include <TM_UI.hpp>
#include <TM_CalendarMonthView.hpp>
#include <TM_Task.hpp>

class TM_CalendarView : public TM_View
{
    public:
        TM_CalendarView(SkRect bounds, std::multiset<TM_Task>* tasks);
    private:
        TM_CalendarMonthView<TM_CalendarView>* monthView;
        TM_CalendarWeekView* weekView;
		TM_HorizontalView* buttonBar;
		TM_View* vitalView;
        std::chrono::year_month_day currentDate;
        std::multiset<TM_Task>* tasks;
};
