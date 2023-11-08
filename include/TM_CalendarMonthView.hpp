#pragma once

#include <TM_Core.hpp>
#include <TM_MonthView.hpp>
#include <TM_Button.hpp>
#include <TM_HorizontalView.hpp>
#include <TM_View.hpp>

template<typename ContextType> class TM_CalendarMonthView : public TM_View
{
    public:
        TM_CalendarMonthView(SkRect bounds=SkRect::MakeEmpty(), ContextType* contextPtr=NULL, void (*setDateFunc)(ContextType* contextPtr, TM_YMD date)=NULL, TM_YMD (*getDateFunc)(ContextType* contextPtr)=NULL, TM_ViewSetting viewSetting={colorScheme[0],colorScheme[3],colorScheme[3],1,16,0,0});
       
        void setDate(TM_YMD date);

    private:
        TM_MonthView<ContextType> monthView;
        TM_TextView dataView;
        TM_HorizontalView weekDayLabels;
        TM_Button<TM_MonthView<ContextType>,int> nextMonth, previousMonth;
};