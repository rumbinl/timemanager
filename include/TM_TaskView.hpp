#pragma once

#include <TM_UI.hpp>
#include <TM_CalendarMonthView.hpp>
#include <TM_Task.hpp>

class TM_TaskView : public TM_View
{
    public:
        TM_TaskView(SkRect bounds, std::set<TM_Task>* tasksPtr,std::map<std::chrono::year_month_day,int>* calendarPtr);
        void setTask(TM_Task* task);
        void setDate(std::chrono::year_month_day date);
        std::chrono::year_month_day getDate();
		std::string getText();
        std::map<std::chrono::year_month_day,int>* getCalendarPtr();
        TM_Task* getTask();
    private:
        TM_Task* currentTask=NULL; 
        TM_TextBox* textBox;
        TM_View* taskList;
        TM_Button<TM_TaskView>* addSubtaskButton,*newTaskButton;
        TM_CalendarMonthView<TM_TaskView>* monthView;
        std::chrono::year_month_day date;
        std::set<TM_Task>* tasksPtr;
        std::map<std::chrono::year_month_day,int>* calendarPtr;
};
