#pragma once

#include <TM_HorizontalView.hpp>
#include <TM_View.hpp>
#include <TM_CalendarWeekView.hpp>
#include <TM_CalendarMonthView.hpp>
#include <TM_Button.hpp>
#include <TM_Task.hpp>
#include <TM_TaskManager.hpp>

class TM_CalendarView : public TM_View
{
    public:
        TM_CalendarView(SkRect bounds, TM_TaskManager* taskManPtr);
    private:
        TM_CalendarMonthView<TM_CalendarView>* monthView;
        TM_CalendarWeekView* weekView;
		TM_HorizontalView* buttonBar;
		TM_View* vitalView;
        std::chrono::year_month_day currentDate;
        TM_TaskManager* taskManPtr;
};
