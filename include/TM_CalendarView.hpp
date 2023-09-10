#pragma once

#include <TM_UI.hpp>
#include <TM_CalendarMonthView.hpp>
#include <TM_Task.hpp>

class TM_CalendarView : public TM_View
{
    public:
        TM_CalendarView(SkRect bounds, std::set<TM_Task>* tasks);
		TM_HorizontalView* getBarView();
    private:
        TM_CalendarMonthView<TM_CalendarView>* monthView;
        TM_CalendarWeekView* weekView;
		TM_HorizontalView* buttonBar;
        std::chrono::year_month_day currentDate;
        std::set<TM_Task>* tasks;
};
