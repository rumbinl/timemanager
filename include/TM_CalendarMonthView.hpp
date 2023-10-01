#pragma once

#include <TM_Core.hpp>
#include <TM_RenderObject.hpp>
#include <TM_TextView.hpp>

class TM_CalendarMonthView : public TM_RenderObject
{
    public:
        TM_CalendarMonthView(SkRect bounds, std::chrono::year_month_day* datePtr=NULL, TM_ViewSetting viewSetting={colorScheme[0],colorScheme[3],colorScheme[3],1,16,0,0});
        void Render(TM_RenderInfo renderInfo) override;
        bool PollEvents(TM_EventInput eventInput) override;
        void setMonth(std::chrono::year_month ym);
        void setDate(std::chrono::year_month_day date);
        void setDatePtr(std::chrono::year_month_day* datePtr);
        std::chrono::year_month getMonthYear();
        ~TM_CalendarMonthView();
    private:
        int firstDay,numDays,numRows,numColumns = 7.0f,hoverDayButton=-1,selectDayButton=-1;
        TM_TextView* dataView;
        std::chrono::year_month_day* datePtr, datePlaceholder;
        std::vector<TM_TextView> weekList,dayViewList;
        std::chrono::year_month month;
};