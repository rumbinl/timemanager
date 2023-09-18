#pragma once

#include <string>
#include <set>

#include <TM_Task.hpp>

class TM_Calendar 
{
    public:
        TM_Calendar(std::string path);
        void loadCalendarData(std::string data);
    private:
        std::set<TM_Task*,TM_TaskPtrCompare> taskSet;
};