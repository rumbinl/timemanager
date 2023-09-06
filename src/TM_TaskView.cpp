#include <TM_TaskView.hpp>

TM_TaskView::TM_TaskView(SkRect bounds) : TM_View(bounds, {})
{
    this->textBox = new TM_TextBox(SkRect::MakeWH(0,40),"Untitled");
    this->taskList = new TM_View(SkRect::MakeWH(0,200), {});
    this->addSubtaskButton = new TM_Button<TM_TaskView>("Add Subtask", SkRect::MakeWH(0,50), [](TM_TaskView* context) {
        if(context->getTask())
            context->getTask()->addSubtask("Subtask", {0,30});
    },this);
    //this->newTaskButton = new TM_Button<TM_TaskView>("Create Task", SkRect::MakeWH(0,50));
    this->monthView = new TM_CalendarMonthView<TM_TaskView>(SkRect::MakeWH(0,480), 
        [](TM_TaskView* context, std::chrono::year_month_day date) {
            if(context->getTask())
                context->getTask()->setDate(date);
        },
    this);

    this->renderObjects.push_back(this->textBox);
    this->renderObjects.push_back(this->taskList);
    this->renderObjects.push_back(this->addSubtaskButton);
    this->renderObjects.push_back(this->monthView);
}

void TM_TaskView::setTask(TM_Task* task)
{
    this->currentTask = task;
}

TM_Task* TM_TaskView::getTask()
{
    return this->currentTask;
}