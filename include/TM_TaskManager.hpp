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

typedef std::multiset<TM_Task*,TM_Task::TM_TaskIDCompare> TM_TaskSet;
typedef std::multiset<TM_Task*,TM_Task::TM_TaskIDCompare>::iterator TM_TaskIt;
typedef std::multiset<TM_TaskIt,TM_Task::TM_TaskItCompare> TM_TaskItSet;
typedef std::multiset<TM_TaskIt,TM_Task::TM_TaskItCompare>::iterator TM_TaskItIt;
typedef std::multiset<TM_TaskItIt,TM_Task::TM_SubtaskItCompare>::iterator TM_SubtaskIt;

struct TM_FreeDay 
{
    TM_Time freeTime, maxTimeSlot;
    TM_YMD date;
    bool operator<(const TM_FreeDay& b) const
    {
        return freeTime>b.freeTime;
    }
};

class TM_TaskManager 
{
	public:
		TM_TaskManager(TM_TaskView** outputPtr, TM_StorageManager** storageManPtr);
        TM_TaskItIt addTask(TM_Task* task);
        void addSubtask(TM_TaskItIt task);
        void scheduleTask(TM_TaskItIt task, TM_Task *headTask);
        void setDateTime(TM_YMD startDate=ZeroDate, TM_Time startTime=ZeroTime, TM_YMD endDate=ZeroDate, TM_Time endTime=ZeroTime);
        void openDocXFile(std::string filePath);
        void initializeSubtasks();
        TM_Time getStartTime();
        TM_Time getEndTime();
        void setTaskName(std::string taskName);
        TM_Task* getCurrentTask();
        TM_TaskItIt getStartIt();
        TM_TaskItIt getEndIt();
        TM_TaskItIt getCurrentTaskIt();
        TM_TaskItIt getTaskByID(int id);
        void setTaskProgress(TM_Time progress);
        void deleteCurrentTask();
        void deleteTask(TM_TaskItIt taskIt);
        void setRepeat(int repeat);
        void setColor(SkColor color);
        int getRepeat();
		TM_TaskItSet& getTaskList();
        void setCurrentTask(TM_TaskItIt currentTask);
	private:
        bool isSchedulable(TM_Task* task, TM_YMD date); 

		TM_TaskSet databaseSortedTasks;
        TM_TaskItSet dateSortedTasks;
        TM_TaskItIt currentTask=dateSortedTasks.end();

		std::map<TM_YMD,int> freeTimeMap;
        TM_TaskView** outputPtr;
        TM_StorageManager** storageManPtr;
};
