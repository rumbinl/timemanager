#pragma once

#include <TM_UI.hpp>
#include <TM_CalendarMonthView.hpp>

class TM_CalendarView : public TM_View
{
    public:
        TM_CalendarView(SkRect bounds);
        ~TM_CalendarView();
    private:
        TM_CalendarMonthView<TM_CalendarView>* monthView;
        TM_CalendarWeekView* weekView;
        std::chrono::year_month_day currentDate;
};