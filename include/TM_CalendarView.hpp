#pragma once

#include <TM_HorizontalView.hpp>
#include <TM_View.hpp>
#include <TM_CalendarWeekView.hpp>
#include <TM_CalendarMonthView.hpp>
#include <TM_Button.hpp>
#include <TM_Task.hpp>
#include <TM_TaskManager.hpp>
#include <TM_TaskView.hpp>

class TM_CalendarView : public TM_View
{
    public:
        TM_CalendarView(SkRect bounds, TM_TaskManager* taskManPtr);
        void setReferenceDate(TM_YMD date);
        TM_YMD getReferenceDate();
    private:
        TM_CalendarMonthView<TM_CalendarView>* monthView;
        TM_CalendarWeekView* weekView;
		TM_HorizontalView* buttonBar;
		TM_View* vitalView;
        TM_YMD currentDate;
        TM_TaskManager* taskManPtr;
};
