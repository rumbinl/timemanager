#pragma once

#include <TM_Core.hpp>
#include <TM_RenderObject.hpp>
#include <TM_TaskManager.hpp>
#include <TM_SelectButton.hpp>

template<typename ContextType> class TM_MonthView : public TM_RenderObject
{
    public:
        TM_MonthView(SkRect bounds=SkRect::MakeEmpty(), ContextType* contextPtr=NULL, void (*setDateFunc)(ContextType* contextPtr, TM_YMD date)=NULL, TM_YMD (*getDateFunc)(ContextType* contextPtr)=NULL, TM_ViewSetting viewSetting={colorScheme[0],colorScheme[3],colorScheme[3],0,16,0,0});
       
        void Render(TM_RenderInfo renderInfo) override;

        bool PollEvents(TM_EventInput eventInput) override;

        void setDate(TM_YMD date);

        TM_YMD getDate();

        std::chrono::year_month getMonthYear();

    private:
        int numColumns = 7;
        ContextType* contextPtr;
        void (*setDateFunc)(ContextType* contextPtr, TM_YMD date);
        TM_YMD (*getDateFunc)(ContextType* contextPtr);
        TM_YMD datePlaceholder;
        std::vector<TM_SelectButton<TM_MonthView, int> > dayViewList;
        std::chrono::year_month month;
};