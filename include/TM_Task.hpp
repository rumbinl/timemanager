#pragma once

#include <iostream>
#include <vector>
#include <set>
#include <chrono>

#include <TM_DateTime.hpp>

class TM_Task 
{
    public:
        TM_Task(std::string name, std::chrono::year_month_day date, TM_Time time={0,0});
        void addSubtask(std::string name, TM_Time duration);
        void setDate(std::chrono::year_month_day date);
        void setTime(TM_Time time);
        TM_Time getTime();
        std::chrono::year_month_day getDate();
        std::string setName(std::string name);
        void scheduleSubtasks(std::chrono::year_month_day currentDay);
        bool operator<(const TM_Task& b) const;
        ~TM_Task();
    private:
        std::string name; // once it has subtasks the date time variable automatically becomes the deadline for all subtasks
        std::chrono::year_month_day date;
        TM_Time time,duration;
        TM_Task* rootTask;
        std::set<TM_Task> sub_tasks;
};