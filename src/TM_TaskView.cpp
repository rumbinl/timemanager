#include <TM_TaskView.hpp>

TM_TaskView::TM_TaskView(SkRect bounds, TM_TaskManager* taskManPtr, std::map<TM_YMD,int>& calendarPtr) : TM_View(bounds, {},{colorScheme[0],colorScheme[3],colorScheme[3],0,24,0,10})
{
    this->taskManPtr = taskManPtr;
    this->currentTask = &this->dummyTask;
    this->textBox = new TM_TextBox<TM_TaskManager>(SkRect::MakeWH(0,48),"New Task", this->taskManPtr, 
                                                   [](TM_TaskManager* taskManager) -> std::string {
                                                        if(taskManager->getCurrentTask()!=NULL)
                                                            return taskManager->getCurrentTask()->getName(); 
                                                        return "";
                                                   }, 
                                                   [](TM_TaskManager* taskManager, std::string newString) {
                                                        if(taskManager->getCurrentTask()!=NULL)
                                                            taskManager->setTaskName(newString);
                                                   }, {colorScheme[1],colorScheme[2],colorScheme[3],1,48,5});
    this->taskList = new TM_View(SkRect::MakeWH(0,200), {});
    this->calendarPtr = calendarPtr;
    this->addSubtaskButton = new TM_Button<TM_TaskView, int>("Add Subtask", SkRect::MakeWH(0,50), 0, this, [](TM_TaskView* context, int data) {
    });

    this->scheduleTaskButton = new TM_Button<TM_TaskView, int>("Schedule", SkRect::MakeWH(0,50), 0, this, [](TM_TaskView* context, int data) {
        // TODO: Add Task
	});

    this->deleteTaskButton = new TM_Button<TM_TaskView, int>("Delete this task", SkRect::MakeWH(0,50), 0, this, [](TM_TaskView* context, int data) {
        context->getTaskManPtr()->deleteCurrentTask();
    });

    this->startDateMonthView = new TM_CalendarMonthView<TM_TaskManager>(SkRect::MakeWH(0,350), this->taskManPtr, 

        [](TM_TaskManager* taskMan, TM_YMD date) {
            if(taskMan->getCurrentTask()!=NULL)
                taskMan->setStartDateTime(date, ZeroTime);
        }, 

        [](TM_TaskManager* taskMan) -> TM_YMD {
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
    this->renderObjects.push_back(new TM_Button<TM_TaskView, int>("Hide Task Panel", SkRect::MakeWH(0,50), 0, this, [](TM_TaskView* taskView, int data){
        taskView->setExistence(false);
    }));
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
    return &this->calendarPtr;
}

void TM_TaskView::setTask(TM_Task* taskPtr)
{
}

void TM_TaskView::Render(TM_RenderInfo renderInfo)
{
    TM_View::Render(renderInfo);
}

bool TM_TaskView::PollEvents(TM_EventInput eventInput)
{
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
