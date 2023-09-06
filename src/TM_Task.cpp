#include <TM_Task.hpp>

TM_Task::TM_Task(std::string name, std::chrono::year_month_day date, TM_Time time)
{
    this->name = name;
    this->date = date;
    this->time = time;
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

std::string TM_Task::setName(std::string name)
{
    return this->name;
}

void TM_Task::scheduleSubtasks(std::chrono::year_month_day currentDay)
{
    if(this->sub_tasks.empty())
        return;
    for(const TM_Task& subtask : this->sub_tasks)
    {
    }
}

bool TM_Task::operator<(const TM_Task& b) const 
{
    return false;
}

TM_Task::~TM_Task()
{

}