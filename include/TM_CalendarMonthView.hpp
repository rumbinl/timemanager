#pragma once

#include <TM_Core.hpp>
#include <TM_MonthView.hpp>
#include <TM_Button.hpp>
#include <TM_HorizontalView.hpp>
#include <TM_View.hpp>

class TM_CalendarMonthView : public TM_View
{
    public:
        TM_CalendarMonthView(SkRect bounds=SkRect::MakeEmpty(), void* contextPtr=NULL, void (*setDateFunc)(void* contextPtr, TM_YMD date)=NULL, TM_YMD (*getDateFunc)(void* contextPtr)=NULL, TM_ViewSetting viewSetting={colorScheme[0],colorScheme[3],colorScheme[3],0,16,0,0});

        std::chrono::year_month getMonthYear();

        void setDate(TM_YMD date);

        SkRect getSaveBounds();

        void switchNavigators();
    private:
        TM_MonthView* monthView;
        TM_HorizontalView controlPanel;
        SkRect saveMaxBounds;
        TM_Button<int> nextMonth, previousMonth, dataView;
};