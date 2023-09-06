#pragma once

#include <TM_UI.hpp>
#include <TM_CalendarMonthView.hpp>
#include <TM_Task.hpp>

class TM_TaskView : public TM_View
{
    public:
        TM_TaskView(SkRect bounds);
    private:
        TM_Task* currentTask=NULL; 
        TM_TextBox* textBox;
        TM_View* taskList;
        TM_Button<TM_TaskView>* addSubtaskButton;
        TM_CalendarMonthView<TM_TaskView>* monthView;
};