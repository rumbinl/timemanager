#include <TM_TaskView.hpp>

TM_TaskView::TM_TaskView(SkRect bounds, TM_TaskManager* taskManPtr, std::map<TM_YMD,int>* calendarPtr) : TM_View(bounds, {},{colorScheme[0],colorScheme[3],colorScheme[3],0,24,0,10})
{
    this->taskManPtr = taskManPtr;
    this->currentTask = &this->dummyTask;
    this->textBox = new TM_TextBox(SkRect::MakeWH(0,48),"New Task", NULL, {colorScheme[1],colorScheme[2],colorScheme[3],1,48,5});
    this->taskList = new TM_View(SkRect::MakeWH(0,200), {});
    this->calendarPtr = calendarPtr;
    this->addSubtaskButton = new TM_Button<TM_TaskView>("Add Subtask", SkRect::MakeWH(0,50), [](TM_TaskView* context) {
    },this);

    this->scheduleTaskButton = new TM_Button<TM_TaskView>("Schedule", SkRect::MakeWH(0,50), [](TM_TaskView* context) {
        // TODO: Add Task
	}, this);

    this->deleteTaskButton = new TM_Button<TM_TaskView>("Delete this task", SkRect::MakeWH(0,50), [](TM_TaskView* context) {
        context->getTaskManPtr()->deleteCurrentTask();
    },this);

    this->startDateMonthView = new TM_CalendarMonthView<TM_TaskManager>(SkRect::MakeWH(0,350), this->taskManPtr, 

        [](TM_TaskManager* taskMan, TM_YMD date) {
            if(taskMan->getCurrentTask()!=NULL)
                taskMan->setStartDateTime(date, taskMan->getCurrentTask()->getStartTime());
        }, 

        [](TM_TaskManager* taskMan) {
            if(taskMan->getCurrentTask()==NULL)
                return ZeroDate;
            return taskMan->getCurrentTask()->getStartDate();
        }
    );

    this->endDateMonthView = new TM_CalendarMonthView<TM_TaskManager>(SkRect::MakeWH(0,350), this->taskManPtr, 

        [](TM_TaskManager* taskMan, TM_YMD date) {
            if(taskMan->getCurrentTask()!=NULL)
            {
                taskMan->setEndDateTime(date, taskMan->getCurrentTask()->getEndTime());
            }
        },

        [](TM_TaskManager* taskMan) {
            if(taskMan->getCurrentTask() == NULL)
                return ZeroDate;
            return taskMan->getCurrentTask()->getEndDate();
        } 

    );

    this->startDateLabel = new TM_TextView("Start Date", SkRect::MakeWH(0,50));
    this->endDateLabel = new TM_TextView("End Date", SkRect::MakeWH(0,50));

    this->renderObjects.push_back(this->textBox);
    this->renderObjects.push_back(new TM_HorizontalView(SkRect::MakeWH(0,50), {
        this->startDateLabel,
        this->endDateLabel 
    }));

    this->renderObjects.push_back(new TM_HorizontalView(SkRect::MakeWH(0,350), {
        new TM_TimeDial<TM_TaskManager>(SkRect::MakeWH(0,0), this->taskManPtr, [](TM_TaskManager* taskManPtr, TM_Time time) {
            taskManPtr->setStartDateTime(ZeroDate, time);
        }, 
        [](TM_TaskManager* taskManPtr) -> TM_Time
        {
            return taskManPtr->getStartTime();
        }),
        this->startDateMonthView, 
        new TM_TimeDial<TM_TaskManager>(SkRect::MakeWH(0,0), this->taskManPtr, [](TM_TaskManager* taskManPtr, TM_Time time) {
            taskManPtr->setEndDateTime(ZeroDate, time);
        }, 
        [](TM_TaskManager* taskManPtr) -> TM_Time
        {
            return taskManPtr->getEndTime();
        }),
        this->endDateMonthView
    }));
    //this->renderObjects.push_back(this->taskList);
    this->renderObjects.push_back(this->addSubtaskButton);
    //this->renderObjects.push_back(this->scheduleTaskButton);
    this->renderObjects.push_back(this->deleteTaskButton);
}

void TM_TaskView::setDate(TM_YMD date)
{
    this->date = date;
}

TM_YMD TM_TaskView::getDate()
{
    return this->date;
}

std::string TM_TaskView::getText()
{
	return this->textBox->getText();
}

std::map<TM_YMD,int>* TM_TaskView::getCalendarPtr()
{
    return this->calendarPtr;
}

void TM_TaskView::setTask(TM_Task* taskPtr)
{
}

void TM_TaskView::Render(TM_RenderInfo renderInfo)
{
    this->SynchronizeView();
    TM_View::Render(renderInfo);
}

void TM_TaskView::SynchronizeView()
{
    if(this->taskManPtr->getCurrentTask() != NULL)
    {
        TM_Task* currentTaskPtr = this->taskManPtr->getCurrentTask();
        this->textBox->setSrcString(currentTaskPtr->getNamePtr());
        this->startDateLabel->setText("Start Date" + TM_DateToString(currentTaskPtr->getStartDate()));
        this->endDateLabel->setText("End Date" + TM_DateToString(currentTaskPtr->getEndDate()));
    }
}

bool TM_TaskView::PollEvents(TM_EventInput eventInput)
{
    this->SynchronizeView(); 
    return TM_View::PollEvents(eventInput);
}

TM_Task* TM_TaskView::getTask()
{
    return this->taskManPtr->getCurrentTask();
}

TM_TaskManager* TM_TaskView::getTaskManPtr()
{
    return this->taskManPtr;
}