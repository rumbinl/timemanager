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
    return *this->currentTask;
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

