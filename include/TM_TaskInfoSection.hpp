#pragma once

#include <TM_HorizontalView.hpp>
#include <TM_TaskManager.hpp>
#include <TM_Button.hpp>
#include <TM_TextBox.hpp>

class TM_TaskInfoSection : public TM_HorizontalView
{
    public:
        TM_TaskInfoSection(SkRect bounds, TM_TaskManIt iterator, TM_TaskManager* taskManPtr, TM_ViewSetting viewSetting = {colorScheme[1],colorScheme[2],colorScheme[3],0,24,0,10});
        void setTaskIt(TM_TaskManIt taskIt);
    private:
        TM_Button<TM_TaskManIt>* taskName;
        TM_View *startDateView, *endDateView;
        TM_TextView startDate,endDate,startTime,endTime;
};