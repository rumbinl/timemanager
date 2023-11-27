#include <TM_Task.hpp>

TM_Task::TM_Task(TM_Task& copyTask)
{
    *this = copyTask;
}

TM_Task::TM_Task(std::string name, std::chrono::year_month_day startDate, std::chrono::year_month_day endDate, TM_Time startTime,  TM_Time endTime, int dbID, int headTaskID, int repeat, TM_Time progress, uint32_t color)
{
    this->name = name;
    this->startDate = startDate;
    this->startTime = startTime;
    this->endDate = endDate;
    this->endTime = endTime;
    this->dbID = dbID;
    this->headTaskID = headTaskID;
    this->repeat = repeat;
    this->progress = progress;
    this->color = color;
}

void TM_Task::setProgress(TM_Time progress) { this->progress = progress; };
void TM_Task::setStartDate(std::chrono::year_month_day startDate) { this->startDate = startDate; }
void TM_Task::setEndDate(std::chrono::year_month_day endDate) { this->endDate = endDate; }

TM_Time TM_Task::getProgress() { return this->progress; };

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

uint32_t TM_Task::getColor() { return this->color; }
void TM_Task::setColor(uint32_t color) { this->color = color; }

std::multiset<TM_Task::TM_TaskItIt,TM_Task::TM_SubtaskItCompare>& TM_Task::getSubtaskList()
{
    return this->subtasks;
}

int TM_Task::getSubtaskCount()
{
    return this->subtasks.size();
}

void TM_Task::setHeadTaskID(int headTaskID)
{
    this->headTaskID = headTaskID;
}

int TM_Task::getHeadTaskID()
{
    return this->headTaskID;
}

void TM_Task::addSubtask(TM_Task::TM_TaskItIt subtask)
{
    this->subtasks.insert(subtask);
}

void TM_Task::scheduleSubtasks(std::chrono::year_month_day currentDay)
{
    if(this->subtasks.empty())
        return;
    for(TM_TaskItIt subtask : this->subtasks)
    {
    }
}

void TM_Task::setDBID(int dbID)
{
    this->dbID = dbID;
}

TM_Time TM_Task::getTaskLength()
{
    if(startDate == endDate)
    {
        return endTime - startTime;
    }
    else
    {
        std::chrono::days dayDif = std::chrono::sys_days{endDate} - std::chrono::sys_days{startDate};
        return {24 * (dayDif.count()-1) + 24-startTime.hours + endTime.hours, endTime.minutes - startTime.minutes };
    }

}

int TM_Task::getDBID()
{
    return this->dbID;
}

void TM_Task::setRepeat(unsigned repeat)
{
    this->repeat = repeat<0?0:repeat;
}

unsigned TM_Task::getRepeat()
{
    return this->repeat;
}

bool TM_Task::DateRangeInView(TM_YMD viewStartDate, TM_YMD viewEndDate)
{
	return (this->getStartDate() <= viewStartDate && this->getEndDate() >= viewStartDate) || 
	   (this->getStartDate() >= viewStartDate && this->getStartDate() <= viewEndDate);
}

TM_YMD TM_Task::RepeatFirstOccurence(TM_YMD viewStartDate, TM_YMD viewEndDate)
{
	TM_YMD startDate = this->getStartDate(),
		   endDate = this->getEndDate();
	if(this->getStartDate() > viewEndDate)
		return ZeroDate;
	unsigned repeat = this->getRepeat();
	if(startDate < viewStartDate)
	{
		TM_YMD firstOccurrence = std::chrono::year_month_day{std::chrono::sys_days{startDate} + std::chrono::days{(unsigned)(((std::chrono::sys_days{viewStartDate} - std::chrono::sys_days{startDate}).count() -1) /repeat) * repeat}};
		std::chrono::days taskLength = std::chrono::sys_days{endDate} - std::chrono::sys_days{startDate};

		if(std::chrono::sys_days{std::chrono::sys_days{firstOccurrence}+taskLength} >= std::chrono::sys_days{viewStartDate})
		{
			return firstOccurrence;
		}
		else if(std::chrono::sys_days{std::chrono::sys_days{firstOccurrence} + std::chrono::days{repeat}} <= std::chrono::sys_days{viewEndDate})
		{
			return std::chrono::year_month_day{std::chrono::sys_days{std::chrono::sys_days{firstOccurrence} + std::chrono::days{repeat}}};
		}
		return ZeroDate;
	}
	return startDate;
}

TM_YMD TM_Task::RepeatLastOccurence(TM_YMD viewStartDate, TM_YMD viewEndDate)
{
	TM_YMD startDate = this->getStartDate(),
		   endDate = this->getEndDate();
	if(this->getStartDate() > viewEndDate)
		return ZeroDate;
	int repeat = this->getRepeat();

	TM_YMD lastOccurrence = std::chrono::year_month_day{std::chrono::sys_days{startDate} + std::chrono::days{(unsigned)(((std::chrono::sys_days{viewEndDate} - std::chrono::sys_days{startDate}).count()) /repeat) * (unsigned)repeat}};
	if(lastOccurrence >= viewStartDate)
		return lastOccurrence;
	return ZeroDate;
}