#include <TM_TaskView.hpp>

TM_TaskView::TM_TaskView(SkRect bounds, TM_TaskManager* taskManPtr) : TM_View(bounds, {},true,{colorScheme[0],colorScheme[3],colorScheme[3],0,24,10,10})
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
    this->addSubtaskButton = new TM_Button<int>("\uf23a", SkRect::MakeWH(0,50), 0, (void*)this->taskManPtr, [](void* context, int data) {
        TM_TaskManager* taskManPtr = (TM_TaskManager*)context;
        TM_Task* currentTask;
        if((currentTask = taskManPtr->getCurrentTask())!=NULL)
        {
            taskManPtr->addSubtask(taskManPtr->addTask(new TM_Task("Subtask", currentTask->getStartDate(), currentTask->getStartDate(), currentTask->getStartTime(), currentTask->getStartTime())));
        }
    }, {colorScheme[1],colorScheme[2],colorScheme[3],0,24,5,5,true});

    this->scheduleTaskButton = new TM_Button<int>("\ue8b5", SkRect::MakeWH(0,50), 0, (void*)this->taskManPtr, [](void* context, int data) {
        TM_TaskManager* taskMan = (TM_TaskManager*)context;
        if(taskMan->getCurrentTask() != NULL && taskMan->getCurrentTask()->getHeadTaskID() != -1)
        {
            taskMan->scheduleTask(taskMan->getCurrentTaskIt(), **taskMan->getTaskByID(taskMan->getCurrentTask()->getHeadTaskID()));
        }
        // TODO: Add Task
	}, {colorScheme[1],colorScheme[2],colorScheme[3],0,24,5,5,true});

    this->deleteTaskButton = new TM_Button<int>("\ue872", SkRect::MakeWH(0,50), 0, (void*)this->taskManPtr, [](void* context, int data) {
        ((TM_TaskManager*)context)->deleteCurrentTask();
    }, {colorScheme[1],colorScheme[2],colorScheme[3],0,24,5,5,true});

    this->startDateMonthView = new TM_CalendarMonthView(SkRect::MakeWH(0,350), (void*)this->taskManPtr, 

        [](void* taskManPtr, TM_YMD date) {

            TM_TaskManager* taskMan = (TM_TaskManager*)taskManPtr;
            if(taskMan->getCurrentTask()!=NULL)
                taskMan->setDateTime(date);
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
                taskMan->setDateTime(ZeroDate, ZeroTime, date);
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

    this->taskInfoViewPtr = new TM_TaskInfoView(SkRect::MakeWH(0,300), this->taskManPtr);
    this->headTaskViewPtr = new TM_HeadTaskInfoSection(SkRect::MakeWH(0,100), this->taskManPtr);

    this->timerViewPtr = new TM_TimerView(SkRect::MakeWH(0, 250), 50, this->taskManPtr, [](void* contextPtr) -> std::pair<TM_Time,TM_Time> {
        TM_TaskManager* taskManPtr = (TM_TaskManager*)contextPtr;
        TM_Task* currentTask;
        if((currentTask = taskManPtr->getCurrentTask()) != NULL)
            return {currentTask->getProgress(), currentTask->getTaskLength()};
        return {{0,0,1},{0,0,1}};
    }, [](void* contextPtr, TM_Time time) {
        TM_TaskManager* taskManPtr = (TM_TaskManager*)contextPtr;
        taskManPtr->setTaskProgress(time);
    });

    this->renderObjects.push_back(this->textBox);
    this->renderObjects.push_back(new TM_HorizontalView(SkRect::MakeWH(0,50), {
        this->startDateLabel,
        this->endDateLabel 
    }));

    this->renderObjects.push_back(new TM_HorizontalView(SkRect::MakeWH(0,350), {
        new TM_TimeDial(SkRect::MakeWH(0,0), this->taskManPtr, [](void* taskManPtr, TM_Time time) {
            ((TM_TaskManager*)taskManPtr)->setDateTime(ZeroDate, time);
        }, 
        [](void* taskManPtr) -> TM_Time
        {
            return ((TM_TaskManager*)taskManPtr)->getStartTime();
        }),
        this->startDateMonthView, 
        new TM_TimeDial(SkRect::MakeWH(0,0), this->taskManPtr, [](void* taskManPtr, TM_Time time) {
            ((TM_TaskManager*)taskManPtr)->setDateTime(ZeroDate, ZeroTime, ZeroDate, time);
        }, 
        [](void* taskManPtr) -> TM_Time
        {
            return ((TM_TaskManager*)taskManPtr)->getEndTime();
        }),
        this->endDateMonthView
    }));
    
    this->renderObjects.push_back(new TM_Button<bool>("Repetition", SkRect::MakeWH(0,50), false, (void*)this, [](void* context, bool data) {
        TM_TaskView* taskView = (TM_TaskView*)context;
        taskView->setRenderObjectExistence(4,!taskView->getRenderObjectExistence(4));
    }));
    this->renderObjects.push_back(new TM_HorizontalView(SkRect::MakeWH(0,50), {
        new TM_SelectButton<int>("Repeat", SkRect::MakeEmpty(), 0, (void*)this->taskManPtr, [](void* context, int data) {
            TM_TaskManager* taskManPtr = (TM_TaskManager*)context;
            taskManPtr->setRepeat(1);
        }, 
        [](void* context, int data) -> bool {
            TM_TaskManager* taskManPtr = (TM_TaskManager*)context;
            return taskManPtr->getRepeat();
        }), 
        new TM_SelectButton<int>("No Repeat", SkRect::MakeEmpty(), 0, (void*)this->taskManPtr, [](void* context, int data) {
            TM_TaskManager* taskManPtr = (TM_TaskManager*)context;
            taskManPtr->setRepeat(0);
        }, 
        [](void* context, int data) -> bool {
            TM_TaskManager* taskManPtr = (TM_TaskManager*)context;
            return !taskManPtr->getRepeat();
        }),
        new TM_Button<int>("-", SkRect::MakeEmpty(), -1, (void*)this->taskManPtr, [](void* context, int data) {
            TM_TaskManager* taskManPtr = (TM_TaskManager*)context;
            taskManPtr->setRepeat(max((int)taskManPtr->getRepeat()-1,0));
        }),
        new TM_TextView((void*)this->taskManPtr, [](void* context) -> std::string {
            TM_TaskManager* taskManPtr = (TM_TaskManager*)context;
            return std::to_string(taskManPtr->getRepeat())+" days";
        }),
        new TM_Button<int>("+", SkRect::MakeEmpty(), 1, (void*)this->taskManPtr, [](void* context, int data) {
            TM_TaskManager* taskManPtr = (TM_TaskManager*)context;
            taskManPtr->setRepeat(taskManPtr->getRepeat()+1);
        })
    }));
    this->renderObjects.push_back(new TM_Button<bool>("Head Task", SkRect::MakeWH(0,50), false, (void*)this, [](void* context, bool data) {
        TM_TaskView* taskView = (TM_TaskView*)context;
        taskView->setRenderObjectExistence(6,!taskView->getRenderObjectExistence(6));
    }));
    this->renderObjects.push_back(this->headTaskViewPtr);
    this->renderObjects.push_back(new TM_Button<bool>("Subtasks", SkRect::MakeWH(0,50), false, (void*)this, [](void* context, bool data) {
        TM_TaskView* taskView = (TM_TaskView*)context;
        taskView->setRenderObjectExistence(8,!taskView->getRenderObjectExistence(8));
    }));
    this->renderObjects.push_back(this->taskInfoViewPtr);
    this->renderObjects.push_back(new TM_Button<bool>("Countdown", SkRect::MakeWH(0,50), false, (void*)this, [](void* context, bool data) {
        TM_TaskView* taskView = (TM_TaskView*)context;
        taskView->setRenderObjectExistence(10,!taskView->getRenderObjectExistence(10));
    }));
    this->renderObjects.push_back(new TM_View(SkRect::MakeWH(0,300), {0.8,0.2,0.2},{
        this->timerViewPtr, 
        new TM_HorizontalView(SkRect::MakeEmpty(), {
            new TM_Button<bool>([](void* contextPtr) -> std::string {
                    TM_TimerView* timerViewPtr = (TM_TimerView*)contextPtr;
                    if(timerViewPtr->isCountingDown()) 
                        return "\ue034";
                    return "\ue037";
                },
                SkRect::MakeEmpty(), false, (void*)this->timerViewPtr, [](void* contextPtr, bool data) {
                    TM_TimerView* timerViewPtr = (TM_TimerView*)contextPtr;
                    if(timerViewPtr->isCountingDown())
                        timerViewPtr->stopCountdown();
                    else
                        timerViewPtr->startCountdown();
                }, 
            {colorScheme[0],colorScheme[2],SkColorSetRGB(0,0,0xff),0,24,0,0,true}),
            new TM_Button<bool>("\ue877", SkRect::MakeEmpty(), false, (void*)this->timerViewPtr, [](void* contextPtr, bool data) {
                }, {colorScheme[0],colorScheme[2],SkColorSetRGB(0x32,0xde,0x84),0,24,0,0,true})
            })
        })
    );
    this->renderObjects.push_back(new TM_GradientView(SkRect::MakeWH(0,300)));
    this->renderObjects.push_back(new TM_HorizontalView(SkRect::MakeWH(0,50), { this->addSubtaskButton,
    this->scheduleTaskButton,
    this->deleteTaskButton,
    new TM_Button<int>("\ue8f5", SkRect::MakeWH(0,50), 0, this, [](void* taskView, int data){
        ((TM_TaskView*)taskView)->setExistence(false);
    }, {colorScheme[1],colorScheme[2],colorScheme[3],0,24,5,5,true})}));

    this->setRenderObjectExistence(4,false);
    this->setRenderObjectExistence(6,false);
    this->setRenderObjectExistence(8,false);
    this->setRenderObjectExistence(10,false);
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
