#include <TM_TaskManager.hpp>

TM_TaskManager::TM_TaskManager(std::vector<TM_Task*> tasks)
{
    for(TM_Task* task : tasks)
        this->sortedTasks.insert(task);
}

void TM_TaskManager::addTask(TM_Task* task)
{
    this->sortedTasks.insert(task);
    for(TM_Task* subtask : task->getSubtaskList())
        this->sortedTasks.insert(task);
}

std::multiset<TM_Task*,TM_Task::TM_TaskPtrCompare>& TM_TaskManager::getTaskList()
{
    return this->sortedTasks;
}

TM_Task* TM_TaskManager::getCurrentTask()
{
    return (this->currentTask == this->sortedTasks.end())?NULL:*this->currentTask;
}

void TM_TaskManager::deleteCurrentTask()
{
    if(this->currentTask!=this->sortedTasks.end())
        this->sortedTasks.erase(this->currentTask);
    this->currentTask = this->sortedTasks.end();
}

void TM_TaskManager::setCurrentTask(std::multiset<TM_Task*,TM_Task::TM_TaskPtrCompare>::iterator currentTask)
{
    this->currentTask = currentTask;
}

void TM_TaskManager::setStartDateTime(TM_YMD startDate, TM_Time startTime)
{
    if(!this->getCurrentTask() || (this->getCurrentTask()->getStartDate()==this->getCurrentTask()->getEndDate()&&this->getCurrentTask()->getEndTime()<startTime))
        return;
    TM_Task* tempTask = new TM_Task(**(this->currentTask));
    delete *this->currentTask;
    this->sortedTasks.erase(this->currentTask);
    if(startDate != ZeroDate)
        tempTask->setStartDate(startDate);
    if(startTime != ZeroTime)
        tempTask->setStartTime(startTime);
    this->currentTask = this->sortedTasks.insert(tempTask);
}

void TM_TaskManager::setEndDateTime(TM_YMD endDate, TM_Time endTime)
{
    if(this->currentTask == this->sortedTasks.end() || (this->getCurrentTask()->getStartDate()==this->getCurrentTask()->getEndDate()&&endTime<this->getCurrentTask()->getStartTime()))
        return;
    TM_Task* tempTask = new TM_Task(**(this->currentTask));
    delete *this->currentTask;
    this->sortedTasks.erase(this->currentTask);
    if(endDate != ZeroDate)
        tempTask->setEndDate(endDate);
    if(endTime != ZeroTime)
        tempTask->setEndTime(endTime);
    this->currentTask = this->sortedTasks.insert(tempTask);
}

void TM_TaskManager::setTaskName(std::string taskName)
{
    (*this->currentTask)->setName(taskName);
}

TM_Time TM_TaskManager::getStartTime()
{
    if(!getCurrentTask())
        return {0,0};
    return getCurrentTask()->getStartTime();
}

TM_Time TM_TaskManager::getEndTime()
{
    if(!getCurrentTask())
        return {0,0};
    return getCurrentTask()->getEndTime();
}
