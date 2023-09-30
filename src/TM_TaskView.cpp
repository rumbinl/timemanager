#include <TM_TaskView.hpp>

TM_TaskView::TM_TaskView(SkRect bounds, TM_TaskManager* taskManPtr, std::map<std::chrono::year_month_day,int>* calendarPtr) : TM_View(bounds, {},{colorScheme[0],colorScheme[3],colorScheme[3],0,24,0,10})
{
    this->taskManPtr = taskManPtr;
    this->textBox = new TM_TextBox(SkRect::MakeWH(0,48),"New Task", {colorScheme[1],colorScheme[2],colorScheme[3],1,48,5});
    this->taskList = new TM_View(SkRect::MakeWH(0,200), {});
    this->calendarPtr = calendarPtr;
    this->addSubtaskButton = new TM_Button<TM_TaskView>("Add Subtask", SkRect::MakeWH(0,50), [](TM_TaskView* context) {
        if(context->getTask())
            context->getTask()->addSubtask("Subtask", {0,30});
    },this);

    this->scheduleTaskButton = new TM_Button<TM_TaskView>("Schedule", SkRect::MakeWH(0,50), [](TM_TaskView* context) {
        // TODO: Add Task
	}, this);

    this->deleteTaskButton = new TM_Button<TM_TaskView>("Delete this task", SkRect::MakeWH(0,50), [](TM_TaskView* context) {
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
    this->renderObjects.push_back(this->scheduleTaskButton);
    this->renderObjects.push_back(this->deleteTaskButton);
}

void TM_TaskView::setDate(std::chrono::year_month_day date)
{
    this->date = date;
}

std::chrono::year_month_day TM_TaskView::getDate()
{
    return this->date;
}

std::string TM_TaskView::getText()
{
	return this->textBox->getText();
}

std::map<std::chrono::year_month_day,int>* TM_TaskView::getCalendarPtr()
{
    return this->calendarPtr;
}

void TM_TaskView::setTask(TM_Task* taskPtr)
{
}

TM_Task* TM_TaskView::getTask()
{
    return this->currentTask;
}
