#include <TM_Task.hpp>

TM_Task::TM_Task(std::string name, std::chrono::year_month_day date, TM_Time time)
{
    this->name = name;
    this->date = date;
    this->time = time;
}

TM_Task::TM_Task(std::string name, std::chrono::year_month_day date, std::map<std::chrono::year_month_day,int>* calendarPtr)
{
    int numDays = (std::chrono::sys_days(date) - std::chrono::sys_days(getCurrentDate())).count();
    std::chrono::year_month_day optimalDate = getCurrentDate();
    int optimalCost = (*calendarPtr)[optimalDate];
    for(int i=1;i<numDays;i++)
    {
        std::chrono::year_month_day currentDate = std::chrono::sys_days(getCurrentDate()) + std::chrono::days{i};
        if((*calendarPtr)[currentDate]<=optimalCost)
        {
            optimalDate=currentDate;
            optimalCost=(*calendarPtr)[currentDate];
        }
    }
    this->name = name;
    this->date = optimalDate;
    this->time = {0,0};
    (*calendarPtr)[optimalDate] += 30;
}

void TM_Task::addSubtask(std::string name, TM_Time duration)
{
    
}

void TM_Task::setDate(std::chrono::year_month_day date)
{
    this->date = date;
}

void TM_Task::setTime(TM_Time time)
{
    this->time = time;
}

TM_Time TM_Task::getTime()
{
    return this->time;
}

std::chrono::year_month_day TM_Task::getDate()
{
    return this->date;
}

void TM_Task::setName(std::string name)
{
    this->name = name;
}

std::string TM_Task::getName()
{
    return this->name;
}

void TM_Task::scheduleSubtasks(std::chrono::year_month_day currentDay)
{
    if(this->sub_tasks.empty())
        return;
    for(TM_Task* subtask : this->sub_tasks)
    {
    }
}

bool TM_Task::operator<(const TM_Task& b) const 
{
    return true;
}

TM_Task::~TM_Task()
{

}
