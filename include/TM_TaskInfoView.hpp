#pragma once

#include <TM_View.hpp>
#include <TM_TextView.hpp>
#include <TM_TaskManager.hpp>
#include <TM_Button.hpp>
#include <TM_HorizontalView.hpp>
#include <TM_Task.hpp>
#include <TM_RuntimeInfo.hpp>
#include <set>

class TM_TaskInfoView : public TM_HorizontalView
{
    public:
        TM_TaskInfoView(SkRect bounds = SkRect::MakeEmpty(), TM_TaskManager* taskManPtr=NULL, std::pair<TM_TaskManIt,TM_TaskManIt> (*getItFunc)(TM_TaskManager* taskManPtr)=NULL,  TM_ViewSetting viewSetting = {colorScheme[1],colorScheme[2],colorScheme[3],0,24,0,10});
        void Render(TM_RenderInfo renderInfo) override;
        bool PollEvents(TM_EventInput eventInput) override;
    private:
        TM_Button<TM_TaskManager,TM_TaskManIt>* taskName;
        TM_View *startDateView, *endDateView;
        TM_TextView startDate,endDate,startTime,endTime;
        SkScalar blockHeight = 0.0f;
        TM_TaskManager* taskManPtr;
        std::pair<TM_TaskManIt,TM_TaskManIt> (*getItFunc)(TM_TaskManager* taskManPtr);
};