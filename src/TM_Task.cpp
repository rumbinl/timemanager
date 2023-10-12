#include <TM_Task.hpp>

TM_Task::TM_Task(TM_Task& copyTask)
{
    *this = copyTask;
}

TM_Task::TM_Task(std::string name, std::chrono::year_month_day startDate, std::chrono::year_month_day endDate, TM_Time startTime, TM_Time endTime)
{
    this->name = name;
    this->startDate = startDate;
    this->startTime = startTime;
    this->endDate = endDate;
    this->endTime = endTime;
}

TM_Task::TM_Task(std::string name, std::chrono::year_month_day startDate, std::chrono::year_month_day deadline, TM_Time startTime, TM_Time endTime, TM_Time duration, std::map<std::chrono::year_month_day,int>* calendarPtr)
{
    this->name = name;
    int numDays = (std::chrono::sys_days(deadline) - std::chrono::sys_days(startDate)).count();
    std::chrono::year_month_day optimalDate = startDate;
    int optimalCost = (*calendarPtr)[optimalDate];
    for(int i=1;i<numDays;i++)
    {
        std::chrono::year_month_day currentDate = std::chrono::sys_days(startDate) + std::chrono::days{i};
        if((*calendarPtr)[currentDate]<=optimalCost)
        {
            optimalDate=currentDate;
            optimalCost=(*calendarPtr)[currentDate];
        }
    }
    this->name = name;
    this->startDate = optimalDate;
    (*calendarPtr)[optimalDate] += 30;
}

void TM_Task::addSubtask(std::string name, TM_Time duration)
{
    
}

void TM_Task::setStartDate(std::chrono::year_month_day startDate) { this->startDate = startDate; }
void TM_Task::setEndDate(std::chrono::year_month_day endDate) { this->endDate = endDate; }

std::chrono::year_month_day TM_Task::getStartDate() { return std::min(this->startDate, this->endDate); }
std::chrono::year_month_day TM_Task::getEndDate() { return std::max(this->startDate, this->endDate); }

void TM_Task::setStartTime(TM_Time startTime) { this->startTime = startTime; }
void TM_Task::setEndTime  (TM_Time endTime  ) { this->endTime   = endTime  ; }

TM_Time TM_Task::getStartTime() { return this->startDate == this->endDate ? std::min(this->startTime, this->endTime) : (this->startDate < this->endDate ? this->startTime : this->endTime  ); }
TM_Time TM_Task::getEndTime  () { return this->startDate == this->endDate ? std::max(this->startTime, this->endTime) : (this->startDate < this->endDate ? this->endTime   : this->startTime); }

void TM_Task::setName(std::string name) { this->name = name; }
std::string TM_Task::getName() { return this->name; }

std::string* TM_Task::getNamePtr() { return &this->name; }
std::chrono::year_month_day* TM_Task::getStartDatePtr() { return &this->startDate; }
std::chrono::year_month_day* TM_Task::getEndDatePtr() { return &this->endDate; }

std::multiset<TM_Task*,TM_Task::TM_TaskPtrCompare>& TM_Task::getSubtaskList()
{
    return this->subtasks;
}

void TM_Task::scheduleSubtasks(std::chrono::year_month_day currentDay)
{
    if(this->subtasks.empty())
        return;
    for(TM_Task* subtask : this->subtasks)
    {
    }
}

bool TM_Task::operator<(const TM_Task& b) const 
{
    return this->startDate < b.startDate || (this->startDate == b.startDate && this->startTime < b.startTime);
}