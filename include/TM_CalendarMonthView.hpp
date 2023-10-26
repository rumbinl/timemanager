#pragma once

#include <TM_Core.hpp>
#include <TM_RenderObject.hpp>
#include <TM_TextView.hpp>
#include <TM_TaskManager.hpp>

template<typename T> class TM_CalendarMonthView : public TM_RenderObject
{
    public:
        TM_CalendarMonthView(SkRect bounds, T* taskManPtr=NULL, void (*setDateFunc)(T* taskManPtr, TM_YMD date)=NULL, TM_YMD (*getDateFunc)(T* taskManPtr)=NULL, TM_ViewSetting viewSetting={colorScheme[0],colorScheme[3],colorScheme[3],1,16,0,0});
       
        void Render(TM_RenderInfo renderInfo) override;

        bool PollEvents(TM_EventInput eventInput) override;

        void setDate(TM_YMD date);

        std::chrono::year_month getMonthYear();

    private:
        int numColumns = 7.0f,hoverDayButton=-1,selectDayButton=-1;
        TM_TextView* dataView;
        T* datePtr;
        void (*setDateFunc)(T* taskManPtr, TM_YMD date);
        TM_YMD (*getDateFunc)(T* taskManPtr);
        TM_YMD datePlaceholder;
        std::vector<TM_TextView> weekList,dayViewList;
        std::chrono::year_month month;
};