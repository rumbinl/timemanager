#pragma once

#include <TM_Core.hpp>
#include <TM_View.hpp>
#include <TM_TextBox.hpp>
#include <TM_Button.hpp>
#include <TM_CalendarMonthView.hpp>
#include <TM_Task.hpp>
#include <TM_TaskManager.hpp>
#include <TM_HorizontalView.hpp>
#include <TM_TimeDial.hpp>
#include <TM_TaskInfoView.hpp>
#include <TM_TimerView.hpp>
#include <TM_GradientView.hpp>
#include <TM_ApplicationManager.hpp>

class TM_TimerView;

class TM_TaskView : public TM_View
{
    public:
        TM_TaskView(SkRect bounds, TM_TaskManager* taskManPtr, TM_ApplicationManager* appManPtr);
        void setTask(TM_Task* task);
        void setDate(TM_YMD date);
        void Render(TM_RenderInfo renderInfo) override;
        bool PollEvents(TM_EventInput eventInput) override;
        TM_YMD getDate();
		std::string getText();
        TM_Task* getTask();
        TM_TaskManager* getTaskManPtr();
    private:
        TM_Task* currentTask=NULL,dummyTask=TM_Task("",getCurrentDate(),getCurrentDate(),{0,0},{0,0}); 
        TM_TextBox* textBox;
        TM_Button<int>* addSubtaskButton,*scheduleTaskButton,*deleteTaskButton;
        TM_TextView *startDateLabel,*endDateLabel;
        TM_CalendarMonthView *startDateMonthView, *endDateMonthView;
        TM_HeadTaskInfoSection *headTaskViewPtr;
        TM_YMD date;
        TM_TimerView* timerViewPtr;
        TM_TaskManager* taskManPtr;
        TM_TaskInfoView* taskInfoViewPtr;
};