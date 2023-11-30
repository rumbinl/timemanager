#pragma once

#include <TM_Core.hpp>
#include <TM_TaskManager.hpp>
#include <TM_SelectButton.hpp>
#include <TM_View.hpp>
#include <TM_HorizontalView.hpp>

class TM_MonthView : public TM_View
{
    public:
        TM_MonthView(SkRect bounds=SkRect::MakeEmpty(), 
                     void* contextPtr=NULL, 
                     void (*setDateFunc)(void* contextPtr, TM_YMD date)=NULL, 
                     TM_YMD (*getDateFunc)(void* contextPtr)=NULL, 
                     TM_ViewSetting viewSetting={colorScheme[0],colorScheme[3],colorScheme[3],0,16,0,0});
       
        void Render(TM_RenderInfo renderInfo) override;

        bool PollEvents(TM_EventInput eventInput) override;

        void setDate(TM_YMD date);

        void updateDate();

        TM_YMD getDate();

        std::chrono::year_month getMonthYear();

        void setMonthYear(std::chrono::year_month monthYear);

    private:
        int numColumns = 7, idx=0;
        void* contextPtr=NULL;
        void (*setDateFunc)(void* contextPtr, TM_YMD date)=NULL;
        TM_YMD (*getDateFunc)(void* contextPtr)=NULL;
        TM_YMD currentDate;
        std::vector<TM_SelectButton<int> > dayViewList;
        TM_HorizontalView* weekDayLabels;
        std::chrono::year_month month;
};