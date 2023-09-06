#pragma once

#include <TM_UI.hpp>
#include <TM_CalendarMonthView.hpp>
#include <TM_Task.hpp>

class TM_TaskView : public TM_View
{
    public:
        TM_TaskView(SkRect bounds);
        void setTask(TM_Task* task);
        TM_Task* getTask();
    private:
        TM_Task* currentTask=NULL; 
        TM_TextBox* textBox;
        TM_View* taskList;
        TM_Button<TM_TaskView>* addSubtaskButton,*newTaskButton;
        TM_CalendarMonthView<TM_TaskView>* monthView;
};