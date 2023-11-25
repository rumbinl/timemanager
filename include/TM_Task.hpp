#pragma once

#include <iostream>
#include <vector>
#include <chrono>
#include <set>
#include <map>

#include <TM_DateTime.hpp>

class TM_Task 
{
    public:
        TM_Task(TM_Task& copyTask);
        TM_Task(std::string name, std::chrono::year_month_day startDate, std::chrono::year_month_day endDate, TM_Time startTime,  TM_Time endTime, int dbID=-1, int headTaskID=-1, int repeat=0);
        TM_Task(std::string name, std::chrono::year_month_day startDate, std::chrono::year_month_day deadline, TM_Time startTime, TM_Time endTime, TM_Time duration, std::map<std::chrono::year_month_day,int>* calendarPtr);

        void setHeadTaskID(int headTaskID);
        int getHeadTaskID();

        std::string* getNamePtr();

        void setStartDate(std::chrono::year_month_day startDate);
        void setEndDate(std::chrono::year_month_day endDate);

        void setRepeat(int repeat);
        int getRepeat();

        std::chrono::year_month_day* getStartDatePtr();
        std::chrono::year_month_day* getEndDatePtr();

        std::chrono::year_month_day getStartDate();
        std::chrono::year_month_day getEndDate();

        bool operator<(const TM_Task& b) const 
        {
            if(this->startDate == b.startDate)
            {
                if(this->startTime == b.startTime)
                {
                    if(this->endDate == b.endDate)
                    {
                        if(this->endTime == b.endTime)
                        {
                            return this->dbID < b.dbID;
                        }
                        return this->endTime<b.endTime;
                    }
                    return this->endDate < b.endDate;
                }
                return this->startTime < b.startTime;
            }
            return this->startDate < b.startDate;
        }

        bool operator==(const TM_Task& b) const;

        struct TM_TaskPtrCompare { bool operator()(const TM_Task* a, const TM_Task* b) const {
            return *a<*b;
        } };

        typedef std::multiset<TM_Task*,TM_TaskPtrCompare>::iterator TM_TaskIt;

        struct TM_TaskItCompare { 
            bool operator()(const TM_TaskIt& a, const TM_TaskIt& b) const
            {
                return **a<**b;
            } 
        };

        typedef std::multiset<TM_TaskIt,TM_TaskItCompare>::iterator TM_TaskItIt;

        struct TM_SubtaskItCompare { 
            bool operator()(const TM_TaskItIt& a, const TM_TaskItIt& b) const
            {
                return (**a)->getDBID()<(**b)->getDBID();
            } 
        };

        typedef std::multiset<TM_TaskItIt,TM_SubtaskItCompare>::iterator TM_SubtaskIt;

        struct TM_TaskIDCompare {
            bool operator()(const TM_Task* a, const TM_Task* b) const 
            {
                return a->dbID < b->dbID;
            }
        };

        void setStartTime(TM_Time startTime);
        void setEndTime(TM_Time endTime);

        TM_Time getStartTime();
        TM_Time getEndTime();

        TM_Time getTaskLength();

        void setName(std::string name);
        std::string getName();

        void scheduleSubtasks(std::chrono::year_month_day currentDay);

        void addSubtask(TM_TaskItIt subtask);
        int getSubtaskCount();
        std::multiset<TM_TaskItIt,TM_SubtaskItCompare>& getSubtaskList(); 

        void setDBID(int dbID);
        int getDBID();

        bool DateRangeInView(TM_YMD viewStartDate, TM_YMD viewEndDate);
        TM_YMD RepeatFirstOccurence(TM_YMD viewStartDate, TM_YMD viewEndDate);
        TM_YMD RepeatLastOccurence(TM_YMD viewStartDate, TM_YMD viewEndDate);
    private:
        
        std::string name; // once it has subtasks the date time variable automatically becomes the deadline for all subtasks
        std::chrono::year_month_day startDate, endDate; 
        int repeat = 0;
        TM_Time startTime,endTime;

        std::multiset<TM_TaskItIt,TM_SubtaskItCompare> subtasks = {};
        bool locked = true;
        int dbID=-1,headTaskID=-1;
        //TM_TaskIt headTaskIt;
};