#include <TM_TaskManager.hpp>
#include <TM_TaskView.hpp>
#include <TM_Database.hpp>

TM_TaskManager::TM_TaskManager(std::vector<TM_Task*> tasks, TM_TaskView** outputPtr, TM_StorageManager** storageManPtr)
{
    this->outputPtr = outputPtr;
    this->storageManPtr = storageManPtr;
    for(TM_Task* task : tasks)
        this->sortedTasks.insert(task);
}

void TM_TaskManager::addTask(TM_Task* task)
{
    if(this->storageManPtr&&*this->storageManPtr!=NULL && task->getDBID() == -1)
        (*this->storageManPtr)->CreateDBTask(task);
    this->sortedTasks.insert(task);
    for(TM_Task* subtask : task->getSubtaskList())
        this->sortedTasks.insert(task);
}

void TM_TaskManager::addSubtask(TM_Task* task)
{
    if(this->getCurrentTask()) 
    {
        task->setHeadTask(this->getCurrentTask());
        this->getCurrentTask()->addSubtask(task);
        this->sortedTasks.insert(task);
    }
}

TM_TaskManSet& TM_TaskManager::getTaskList()
{
    return this->sortedTasks;
}

TM_Task* TM_TaskManager::getCurrentTask()
{
    return (this->currentTask == this->sortedTasks.end())?NULL:*this->currentTask;
}

void TM_TaskManager::deleteCurrentTask()
{
    if(this->currentTask!=this->sortedTasks.end())
    {
        if(this->storageManPtr&&*this->storageManPtr!=NULL)
            (*this->storageManPtr)->DeleteDBTask(*this->currentTask);
        this->sortedTasks.erase(this->currentTask);
    }
    this->currentTask = this->sortedTasks.end();
}

void TM_TaskManager::setCurrentTask(std::multiset<TM_Task*,TM_Task::TM_TaskPtrCompare>::iterator currentTask)
{
    this->currentTask = currentTask;
    if(this->outputPtr!=NULL)
        (*(this->outputPtr))->setExistence(true);
}

void TM_TaskManager::setStartDateTime(TM_YMD startDate, TM_Time startTime)
{
    if(!this->getCurrentTask() || (this->getCurrentTask()->getStartDate()==this->getCurrentTask()->getEndDate()&&this->getCurrentTask()->getEndTime()<startTime))
        return;
    TM_Task* tempTask = new TM_Task(**(this->currentTask));
    delete *this->currentTask;
    this->sortedTasks.erase(this->currentTask);
    if(startDate != ZeroDate)
        tempTask->setStartDate(startDate);
    if(startTime != ZeroTime)
        tempTask->setStartTime(startTime);
    if(this->storageManPtr&&*this->storageManPtr!=NULL)
        (*this->storageManPtr)->AlterDBTask(tempTask);
    this->currentTask = this->sortedTasks.insert(tempTask);
}

void TM_TaskManager::setEndDateTime(TM_YMD endDate, TM_Time endTime)
{
    if(this->currentTask == this->sortedTasks.end() || (this->getCurrentTask()->getStartDate()==this->getCurrentTask()->getEndDate()&&endTime<this->getCurrentTask()->getStartTime()))
        return;
    TM_Task* tempTask = new TM_Task(**(this->currentTask));
    delete *this->currentTask;
    this->sortedTasks.erase(this->currentTask);
    if(endDate != ZeroDate)
        tempTask->setEndDate(endDate);
    if(endTime != ZeroTime)
        tempTask->setEndTime(endTime);
    if(this->storageManPtr&&*this->storageManPtr!=NULL)
        (*this->storageManPtr)->AlterDBTask(tempTask);
    this->currentTask = this->sortedTasks.insert(tempTask);
}

void TM_TaskManager::setTaskName(std::string taskName)
{
    (*this->currentTask)->setName(taskName);
    if(this->storageManPtr&&*this->storageManPtr!=NULL)
        (*this->storageManPtr)->AlterDBTask(*this->currentTask);
}

int countCells(duckx::TableCell& tableCell)
{
    duckx::TableCell cellNode = tableCell;
    int count = 0;
    while(cellNode.has_next()) { cellNode = cellNode.next(); count++; }
    return count;
}

unsigned monthNameIndex(std::string monthName)
{
    for(unsigned i=0; i<12; i++)
        if(monthName == monthNames[i])
            return i+1; 
    return 0;
}

std::vector<unsigned> extractIntegers(std::string text)
{
    std::stringstream ss;

    ss << text;

    std::vector<unsigned> res = {};

    while(!ss.eof())
    {
        std::string tmp;
        ss >> tmp;
        unsigned day;
        if(std::stringstream(tmp) >> day)
            res.push_back(day);
    }

    return res;
}

void TM_TaskManager::openDocXFile(std::string filePath)
{
    if(!filePath.ends_with(".docx"))
        return;

    duckx::Document doc(filePath);
	doc.open();
	duckx::Table& tableNode = doc.tables();
	duckx::TableRow& tableRow = tableNode.rows();
	int i=0;
	std::chrono::year_month ym;
    if(tableRow.has_next())
        tableRow = tableRow.next();
	while(tableRow.has_next())
	{
		duckx::TableCell& tableCell = tableRow.cells();

		int cellCount = countCells(tableCell);
		if(cellCount == 1)
		{
			duckx::Paragraph& cellText = tableCell.paragraphs();
			duckx::Run& textRun = cellText.runs();
			std::string text = textRun.get_text();
			std::string monthString;
			std::istringstream stream(text);
			int year;
			stream >> monthString >> year;
			ym = {std::chrono::year(year), std::chrono::month(monthNameIndex(monthString))};
		}
		else
		{
			int i=1;
			std::string name = "";
            std::chrono::year_month_day day1,day2;
			while(tableCell.has_next())
			{
				duckx::Paragraph& cellText = tableCell.paragraphs();
				duckx::Run& textRun = cellText.runs();
				std::string text = textRun.get_text();
				if(i==1)
				{
					std::vector<unsigned> days = extractIntegers(text);
					if(days.size())
					{
						day1 = std::chrono::year_month_day{ym.year(), ym.month(), std::chrono::day{days[0]}}, 
                        day2 = std::chrono::year_month_day{ym.year(), ym.month(), std::chrono::day{days.back()}};
						if(days[0]>=days.back() && days.size()>1)
							day2 += std::chrono::months{1};
					}
				}
				else if(i==2)
				{
					name += text + "-";	
				}
				else if(i==3)
				{
					name += text;
                    this->addTask(new TM_Task(name, day1, day2, {0,0}, {24,0}));
				}
				i++;
				tableCell = tableCell.next();
			}
		}
	
		tableRow = tableRow.next();
	}
}

TM_Time TM_TaskManager::getStartTime()
{
    if(!getCurrentTask())
        return {0,0};
    return getCurrentTask()->getStartTime();
}

TM_Time TM_TaskManager::getEndTime()
{
    if(!getCurrentTask())
        return {0,0};
    return getCurrentTask()->getEndTime();
}

TM_TaskManIt TM_TaskManager::getStartIt()
{
    return this->sortedTasks.begin();
}

TM_TaskManIt TM_TaskManager::getEndIt()
{
    return this->sortedTasks.end();
}