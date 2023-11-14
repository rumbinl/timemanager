#pragma once

#include <vector>
#include <iterator>
#include <set>
#include <map>

#include <TM_Core.hpp>
#include <TM_Task.hpp>
#include <duckx/duckx.hpp>

class TM_TaskView;
class TM_StorageManager;

typedef std::multiset<TM_Task*,TM_Task::TM_TaskPtrCompare>::iterator TM_TaskManIt;
typedef std::multiset<TM_Task*,TM_Task::TM_TaskPtrCompare> TM_TaskManSet;

class TM_TaskManager 
{
	public:
		TM_TaskManager(std::vector<TM_Task*> tasks, TM_TaskView** outputPtr, TM_StorageManager** storageManPtr);
        void addTask(TM_Task* task);
        void addSubtask(TM_Task* task);
        void scheduleTask(TM_Task* task, TM_YMD start, TM_YMD end);
        void setStartDateTime(TM_YMD startDate, TM_Time startTime);
        void setEndDateTime(TM_YMD endDate, TM_Time endTime);
        void openDocXFile(std::string filePath);
        TM_Time getStartTime();
        TM_Time getEndTime();
        void setTaskName(std::string taskName);
        TM_Task* getCurrentTask();
        TM_TaskManIt getStartIt();
        TM_TaskManIt getEndIt();
        void deleteCurrentTask();
		TM_TaskManSet& getTaskList();
        void setCurrentTask(TM_TaskManIt currentTask);
	private:
        bool isSchedulable(TM_Task* task, TM_YMD date); 
		TM_TaskManSet sortedTasks;
        TM_TaskManIt currentTask=sortedTasks.end();
		std::map<TM_YMD,int> freeTimeMap;
        TM_TaskView** outputPtr;
        TM_StorageManager** storageManPtr;
};
