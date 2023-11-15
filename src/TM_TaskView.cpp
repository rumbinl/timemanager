#include <TM_TaskView.hpp>

TM_TaskView::TM_TaskView(SkRect bounds, TM_TaskManager* taskManPtr, std::map<TM_YMD,int>& calendarPtr) : TM_View(bounds, {},{colorScheme[0],colorScheme[3],colorScheme[3],0,24,0,10})
{
    this->taskManPtr = taskManPtr;
    this->currentTask = &this->dummyTask;
    this->textBox = new TM_TextBox(SkRect::MakeWH(0,48),"New Task", this->taskManPtr, 
                                                   [](void* taskManagerPtr) -> std::string {
                                                        TM_TaskManager* taskManager = (TM_TaskManager*)taskManagerPtr;
                                                        if(taskManager->getCurrentTask()!=NULL)
                                                            return taskManager->getCurrentTask()->getName(); 
                                                        return "";
                                                   }, 
                                                   [](void* taskManagerPtr, std::string newString) {
                                                        TM_TaskManager* taskManager = (TM_TaskManager*)taskManagerPtr;
                                                        if(taskManager->getCurrentTask()!=NULL)
                                                            taskManager->setTaskName(newString);
                                                   }, {colorScheme[1],colorScheme[2],colorScheme[3],1,48,5});
    this->taskList = new TM_View(SkRect::MakeWH(0,200), {});
    this->calendarPtr = calendarPtr;
    this->addSubtaskButton = new TM_Button<int>("\uf23a", SkRect::MakeWH(0,50), 0, (void*)this, [](void* context, int data) {
    }, {colorScheme[1],colorScheme[2],colorScheme[3],0,24,5,5,true});

    this->scheduleTaskButton = new TM_Button<int>("\ue8b5", SkRect::MakeWH(0,50), 0, this, [](void* context, int data) {
        // TODO: Add Task
	}, {colorScheme[1],colorScheme[2],colorScheme[3],0,24,5,5,true});

    this->deleteTaskButton = new TM_Button<int>("\ue872aa", SkRect::MakeWH(0,50), 0, (void*)this->taskManPtr, [](void* context, int data) {
        ((TM_TaskManager*)context)->deleteCurrentTask();
    }, {colorScheme[1],colorScheme[2],colorScheme[3],0,24,5,5,true});

    this->startDateMonthView = new TM_CalendarMonthView(SkRect::MakeWH(0,350), (void*)this->taskManPtr, 

        [](void* taskManPtr, TM_YMD date) {

            TM_TaskManager* taskMan = (TM_TaskManager*)taskManPtr;
            if(taskMan->getCurrentTask()!=NULL)
                taskMan->setStartDateTime(date, ZeroTime);
        }, 

        [](void* taskManPtr) -> TM_YMD {
            TM_TaskManager* taskMan = (TM_TaskManager*)taskManPtr;
            if(taskMan->getCurrentTask()==NULL)
                return ZeroDate;
            return taskMan->getCurrentTask()->getStartDate();
        }
    );

    this->endDateMonthView = new TM_CalendarMonthView(SkRect::MakeWH(0,350), (void*)this->taskManPtr, 

        [](void* taskManPtr, TM_YMD date) {
            TM_TaskManager* taskMan = (TM_TaskManager*)taskManPtr;
            if(taskMan->getCurrentTask()!=NULL)
            {
                taskMan->setEndDateTime(date, taskMan->getCurrentTask()->getEndTime());
            }
        },

        [](void* taskManPtr) {
            TM_TaskManager* taskMan = (TM_TaskManager*)taskManPtr;
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
        new TM_TimeDial(SkRect::MakeWH(0,0), this->taskManPtr, [](void* taskManPtr, TM_Time time) {
            ((TM_TaskManager*)taskManPtr)->setStartDateTime(ZeroDate, time);
        }, 
        [](void* taskManPtr) -> TM_Time
        {
            return ((TM_TaskManager*)taskManPtr)->getStartTime();
        }),
        this->startDateMonthView, 
        new TM_TimeDial(SkRect::MakeWH(0,0), this->taskManPtr, [](void* taskManPtr, TM_Time time) {
            ((TM_TaskManager*)taskManPtr)->setEndDateTime(ZeroDate, time);
        }, 
        [](void* taskManPtr) -> TM_Time
        {
            return ((TM_TaskManager*)taskManPtr)->getEndTime();
        }),
        this->endDateMonthView
    }));
    //this->renderObjects.push_back(this->taskList);
    this->renderObjects.push_back(this->addSubtaskButton);
    //this->renderObjects.push_back(this->scheduleTaskButton);
    this->renderObjects.push_back(this->deleteTaskButton);
    this->renderObjects.push_back(new TM_Button<int>("\ue8f5", SkRect::MakeWH(0,50), 0, this, [](void* taskView, int data){
        ((TM_TaskView*)taskView)->setExistence(false);
    }, {colorScheme[1],colorScheme[2],colorScheme[3],0,24,5,5,true}));
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
