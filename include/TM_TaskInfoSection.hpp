#pragma once

#include <TM_HorizontalView.hpp>
#include <TM_TaskManager.hpp>
#include <TM_Button.hpp>
#include <TM_TextBox.hpp>

class TM_TaskInfoSection : public TM_HorizontalView
{
    public:
        TM_TaskInfoSection(SkRect bounds, TM_TaskItIt iterator, TM_TaskManager* taskManPtr, TM_ViewSetting viewSetting = {colorScheme[1],colorScheme[2],colorScheme[3],0,24,0,10});
        virtual void setTaskIt(TM_TaskItIt taskIt);
    protected:
        TM_Button<TM_TaskItIt>* taskName,*deleteButton;
        TM_TextView startDate,endDate,startTime,endTime;
};

class TM_ImportTaskInfoSection : public TM_TaskInfoSection
{
    public:
        TM_ImportTaskInfoSection(SkRect bounds, TM_TaskItIt iterator, TM_TaskManager* importTaskManPtr, TM_TaskManager* mainTaskManPtr, TM_ViewSetting viewSetting = {colorScheme[1],colorScheme[2],colorScheme[3],0,24,0,10});
        void setTaskIt(TM_TaskItIt taskIt) override;
    private:
        TM_Button<TM_TaskItIt>* acceptButton,*deleteButton;
        TM_TaskManager* mainTaskManPtr,** taskManPair;
};