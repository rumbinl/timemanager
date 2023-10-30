#pragma once

#include <TM_Core.hpp>
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
        void addSubtask(TM_Task* task);
        void scheduleTask(TM_Task* task, TM_YMD start, TM_YMD end);
        void setStartDateTime(TM_YMD startDate, TM_Time startTime);
        void setEndDateTime(TM_YMD endDate, TM_Time endTime);
        TM_Time getStartTime();
        TM_Time getEndTime();
        void setTaskName(std::string taskName);
        TM_Task* getCurrentTask();
        void deleteCurrentTask();
		std::multiset<TM_Task*,TM_Task::TM_TaskPtrCompare>& getTaskList();
        void setCurrentTask(std::multiset<TM_Task*,TM_Task::TM_TaskPtrCompare>::iterator currentTask);
	private:
        bool isSchedulable(TM_Task* task, TM_YMD date); 
		std::multiset<TM_Task*,TM_Task::TM_TaskPtrCompare> sortedTasks;
        std::multiset<TM_Task*,TM_Task::TM_TaskPtrCompare>::iterator currentTask=sortedTasks.end();
		std::map<TM_YMD,int> freeTimeMap;
};
