#include <TM_Task.hpp>

TM_TaskManager::TM_TaskManager(std::vector<TM_Task*> tasks)
{
    for(TM_Task* task : tasks)
        this->sortedTasks.insert(task);
}