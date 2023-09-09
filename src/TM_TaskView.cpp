#include <TM_TaskView.hpp>

TM_TaskView::TM_TaskView(SkRect bounds, std::set<TM_Task>* tasksPtr, std::map<std::chrono::year_month_day,int>* calendarPtr) : TM_View(bounds, {}, {colorScheme[1],colorScheme[2],colorScheme[3],1,24,30})
{
    this->tasksPtr = tasksPtr;
    this->textBox = new TM_TextBox(SkRect::MakeWH(0,40),"New Task");
    this->taskList = new TM_View(SkRect::MakeWH(0,200), {});
    this->calendarPtr = calendarPtr;
    this->addSubtaskButton = new TM_Button<TM_TaskView>("Add Subtask", SkRect::MakeWH(0,50), [](TM_TaskView* context) {
        if(context->getTask())
            context->getTask()->addSubtask("Subtask", {0,30});
    },this);

    this->newTaskButton = new TM_Button<TM_TaskView>("Create Task", SkRect::MakeWH(0,50), [](TM_TaskView* context) {
        context->tasksPtr->insert(TM_Task("New Task", context->getDate(), context->getCalendarPtr()));
    },this);

    this->monthView = new TM_CalendarMonthView<TM_TaskView>(SkRect::MakeWH(0,480), 
        [](TM_TaskView* context, std::chrono::year_month_day date) {
            context->setDate(date);
        },
    this);

    this->renderObjects.push_back(this->textBox);
    //this->renderObjects.push_back(this->taskList);
    //this->renderObjects.push_back(this->addSubtaskButton);
    this->renderObjects.push_back(this->monthView);
    this->renderObjects.push_back(this->newTaskButton);
}

void TM_TaskView::setDate(std::chrono::year_month_day date)
{
    this->date = date;
}

std::chrono::year_month_day TM_TaskView::getDate()
{
    return this->date;
}

std::map<std::chrono::year_month_day,int>* TM_TaskView::getCalendarPtr()
{
    return this->calendarPtr;
}

TM_Task* TM_TaskView::getTask()
{
    return this->currentTask;
}
