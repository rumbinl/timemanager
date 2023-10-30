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
        TM_Task(std::string name, std::chrono::year_month_day startDate, std::chrono::year_month_day endDate, TM_Time startTime,  TM_Time endTime);
        TM_Task(std::string name, std::chrono::year_month_day startDate, std::chrono::year_month_day deadline, TM_Time startTime, TM_Time endTime, TM_Time duration, std::map<std::chrono::year_month_day,int>* calendarPtr);

        void addSubtask(TM_Task* subtask);

        void setHeadTask(TM_Task* headTask);

        std::string* getNamePtr();

        void setStartDate(std::chrono::year_month_day startDate);
        void setEndDate(std::chrono::year_month_day endDate);

        std::chrono::year_month_day* getStartDatePtr();
        std::chrono::year_month_day* getEndDatePtr();

        std::chrono::year_month_day getStartDate();
        std::chrono::year_month_day getEndDate();

        struct TM_TaskPtrCompare {
            bool operator()(const TM_Task* a, const TM_Task* b) const
            {
                return *a < *b;
            }
        };

        void setStartTime(TM_Time startTime);
        void setEndTime(TM_Time endTime);

        TM_Time getStartTime();
        TM_Time getEndTime();

        void setName(std::string name);
        std::string getName();

        void scheduleSubtasks(std::chrono::year_month_day currentDay);

        std::multiset<TM_Task*,TM_TaskPtrCompare>& getSubtaskList(); 

        bool operator<(const TM_Task& b) const;
        bool operator==(const TM_Task& b) const;
    private:
        
        std::string name; // once it has subtasks the date time variable automatically becomes the deadline for all subtasks
        std::chrono::year_month_day startDate, endDate;
        TM_Time startTime,endTime;
        std::multiset<TM_Task*,TM_TaskPtrCompare> subtasks;
        bool locked = true;
        TM_Task* headTask = NULL;
};

