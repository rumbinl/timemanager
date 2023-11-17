#include <TM_TaskInfoSection.hpp>

TM_TaskInfoSection::TM_TaskInfoSection(SkRect bounds, TM_TaskItIt taskIt, TM_TaskManager* taskManPtr, TM_ViewSetting viewSetting) : TM_HorizontalView(bounds, {}, {}, viewSetting)
{
    this->taskName = new TM_Button<TM_TaskItIt>("", SkRect::MakeEmpty(), taskIt, (void*)taskManPtr, [](void* taskManPtr, TM_TaskItIt task) {
        ((TM_TaskManager*)taskManPtr)->setCurrentTask(task);
    }, {colorScheme[1], colorScheme[2], colorScheme[3], 0, 24, 0, 0}, false);
    this->deleteButton = new TM_Button<TM_TaskItIt>("\ue872aa", SkRect::MakeEmpty(), taskIt, (void*)taskManPtr, [](void* taskManPtr, TM_TaskItIt taskIt) {
        TM_TaskManager* taskMan = (TM_TaskManager*)taskManPtr;
        taskMan->deleteTask(taskIt);
    }, {colorScheme[1], colorScheme[2], colorScheme[3], 0, 24, 0, 0, true});
    this->addRenderObject(taskName);
    this->addRenderObject(new TM_View(SkRect::MakeEmpty(), {0.5,0.5}, {&this->startTime, &this->startDate}, {colorScheme[3],colorScheme[2],colorScheme[1],0,24,0,0}));
    this->addRenderObject(new TM_View(SkRect::MakeEmpty(), {0.5,0.5}, {&this->endTime, &this->endDate}, {colorScheme[3],colorScheme[2],colorScheme[1],0,24,0,0}));
    this->addRenderObject(this->deleteButton);
    this->setTaskIt(taskIt);
}

void TM_TaskInfoSection::setTaskIt(TM_TaskItIt taskIt)
{
    if(**taskIt != NULL)
    {
        TM_Task* task = **taskIt;
        this->taskName->setData(taskIt);
        this->taskName->setText(task->getName());
        this->deleteButton->setData(taskIt);
        this->startDate.setText(TM_GetDateString(task->getStartDate()));
        this->startTime.setText(TM_GetTimeString(task->getStartTime()));
        this->endDate.setText(TM_GetDateString(task->getEndDate()));
        this->endTime.setText(TM_GetTimeString(task->getEndTime()));
    }
}

TM_ImportTaskInfoSection::TM_ImportTaskInfoSection(SkRect bounds, TM_TaskItIt taskIt, TM_TaskManager* importTaskManPtr, TM_TaskManager* mainTaskManPtr, TM_ViewSetting viewSetting) : TM_TaskInfoSection(bounds, taskIt, importTaskManPtr, viewSetting)
{
    this->renderObjects.clear();
    this->mainTaskManPtr = mainTaskManPtr;
    this->taskManPair = (TM_TaskManager**)malloc(2*sizeof(TM_TaskManager*));
    this->taskManPair[0] = mainTaskManPtr;
    this->taskManPair[1] = importTaskManPtr;

    this->acceptButton = new TM_Button<TM_TaskItIt>("\ue5ca", SkRect::MakeEmpty(), taskIt, (void*)taskManPair, [](void* taskManPairPtr, TM_TaskItIt taskIt) {
        TM_TaskManager** taskManPair = (TM_TaskManager**)taskManPairPtr;
        taskManPair[0]->addTask(**taskIt);
        taskManPair[1]->setCurrentTask(taskIt);
        taskManPair[1]->deleteCurrentTask();
    }, {colorScheme[1], colorScheme[2], colorScheme[3], 0, 24, 0, 0, true});

    this->addRenderObject(taskName);
    this->addRenderObject(new TM_View(SkRect::MakeEmpty(), {0.5,0.5}, {&this->startDate, &this->endDate}, {colorScheme[3],colorScheme[2],colorScheme[1],0,24,0,0}));
    this->addRenderObject(new TM_View(SkRect::MakeEmpty(), {0.5,0.5}, {this->acceptButton, this->deleteButton}, {colorScheme[3],colorScheme[2],colorScheme[1],0,24,0,0}));
    this->setTaskIt(taskIt);
}

void TM_ImportTaskInfoSection::setTaskIt(TM_TaskItIt taskIt)
{
    if(**taskIt != NULL)
    {
        this->taskName->setData(taskIt);
        this->taskName->setText((**taskIt)->getName());
        this->startDate.setText(TM_GetDateString((**taskIt)->getStartDate()));
        this->endDate.setText(TM_GetDateString((**taskIt)->getEndDate()));
        this->deleteButton->setData(taskIt);
        this->acceptButton->setData(taskIt);
    }
}