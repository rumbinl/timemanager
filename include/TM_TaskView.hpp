#pragma once

#include <TM_View.hpp>
#include <TM_TextBox.hpp>
#include <TM_Button.hpp>
#include <TM_CalendarMonthView.hpp>
#include <TM_Task.hpp>
#include <TM_TaskManager.hpp>
#include <TM_HorizontalView.hpp>
#include <TM_TimeDial.hpp>

class TM_TaskView : public TM_View
{
    public:
        TM_TaskView(SkRect bounds, TM_TaskManager* taskManPtr,std::map<std::chrono::year_month_day,int>* calendarPtr);
        void setTask(TM_Task* task);
        void setDate(std::chrono::year_month_day date);
        void Render(TM_RenderInfo renderInfo) override;
        bool PollEvents(TM_EventInput eventInput) override;
        void SynchronizeView();
        std::chrono::year_month_day getDate();
		std::string getText();
        std::map<std::chrono::year_month_day,int>* getCalendarPtr();
        TM_Task* getTask();
        TM_TaskManager* getTaskManPtr();
    private:
        TM_Task* currentTask=NULL,dummyTask=TM_Task("",getCurrentDate(),getCurrentDate(),{0,0},{0,0}); 
        TM_TextBox* textBox;
        TM_View* taskList;
        TM_Button<TM_TaskView>* addSubtaskButton,*scheduleTaskButton,*deleteTaskButton;
        TM_TextView *startDateLabel,*endDateLabel;
        TM_CalendarMonthView *startDateMonthView, *endDateMonthView;
        std::chrono::year_month_day date;
        TM_TaskManager* taskManPtr;
        std::map<std::chrono::year_month_day,int>* calendarPtr;
};
