#pragma once

#include <TM_Task.hpp>
#include <vector>
#include <iterator>
#include <set>
#include <map>

class TM_TaskManager 
{
	public:
		TM_TaskManager(std::vector<TM_Task*> tasks);
        void addTask(TM_Task* task);
        void scheduleTask(TM_Task* task, std::chrono::year_month_day start, std::chrono::year_month_day end);
        TM_Task* getCurrentTask();
		std::multiset<TM_Task*,TM_Task::TM_TaskPtrCompare>& getTaskList();
        void setCurrentTask(std::multiset<TM_Task*,TM_Task::TM_TaskPtrCompare>::iterator currentTask);
	private:
        bool isSchedulable(TM_Task* task, std::chrono::year_month_day date); 
		std::multiset<TM_Task*,TM_Task::TM_TaskPtrCompare> sortedTasks;
        std::multiset<TM_Task*,TM_Task::TM_TaskPtrCompare>::iterator currentTask=sortedTasks.end();
		std::map<std::chrono::year_month_day,int> freeTimeMap;
};
