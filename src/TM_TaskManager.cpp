#include <TM_TaskManager.hpp>
#include <TM_TaskView.hpp>
#include <TM_Database.hpp>

TM_TaskManager::TM_TaskManager(TM_TaskView** outputPtr, TM_StorageManager** storageManPtr)
{
    this->outputPtr = outputPtr;
    this->storageManPtr = storageManPtr;
}

TM_TaskItIt TM_TaskManager::addTask(TM_Task* task)
{
    if(this->storageManPtr&&*this->storageManPtr!=NULL && task->getDBID() == -1)
        (*this->storageManPtr)->CreateDBTask(task);
    TM_TaskIt taskIt = this->databaseSortedTasks.insert(task);
    return this->dateSortedTasks.insert(taskIt);
}

void TM_TaskManager::addSubtask(TM_TaskItIt task)
{
    if(this->getCurrentTask()) 
    {
        (**task)->setHeadTaskID(this->getCurrentTask()->getDBID());
        if(this->storageManPtr&&*this->storageManPtr!=NULL)
            (*this->storageManPtr)->AlterDBTask(**task);
        this->getCurrentTask()->addSubtask(task);
    }
}

void TM_TaskManager::initializeSubtasks()
{
    TM_TaskItIt subtaskIt = this->dateSortedTasks.begin();
    while(subtaskIt != this->dateSortedTasks.end())
    {
        int headTaskId;
        if((headTaskId = (**subtaskIt)->getHeadTaskID())!=-1)
        {
            TM_TaskIt headTask = this->databaseSortedTasks.find(new TM_Task("", ZeroDate, ZeroDate, {0,0},{0,0},headTaskId));
            if(headTask != this->databaseSortedTasks.end())
            {
                (*headTask)->addSubtask(subtaskIt);
            }
        }
        subtaskIt ++;
    }
    this->currentTask = dateSortedTasks.end();
}

TM_TaskItSet& TM_TaskManager::getTaskList()
{
    return this->dateSortedTasks;
}

TM_Task* TM_TaskManager::getCurrentTask()
{
    return (this->currentTask == this->dateSortedTasks.end())?NULL:**this->currentTask;

}

TM_TaskItIt TM_TaskManager::getCurrentTaskIt()
{
    return this->currentTask;
}

void TM_TaskManager::deleteTask(TM_TaskItIt taskIt)
{
    if(**taskIt != NULL)
    {
        if(taskIt == this->currentTask)
        {
            this->currentTask = this->dateSortedTasks.end();
        }
        if((**taskIt)->getSubtaskCount()>0)
        {
            TM_SubtaskIt subtaskIt = (**taskIt)->getSubtaskList().begin();
            while(subtaskIt != (**taskIt)->getSubtaskList().end())
            {
                TM_SubtaskIt deleteSubtask = subtaskIt++;
                this->deleteTask(*deleteSubtask);
            }
        }
        if(this->storageManPtr&&*this->storageManPtr!=NULL)
        {
            (*this->storageManPtr)->DeleteDBTask(**taskIt);
        }

        if((**taskIt)->getHeadTaskID() != -1)
        {
            TM_TaskIt headTaskPtr = this->databaseSortedTasks.find(new TM_Task("", ZeroDate, ZeroDate, {0,0},{0,0},(**taskIt)->getHeadTaskID()));
            if(headTaskPtr != this->databaseSortedTasks.end())
            {
                TM_SubtaskIt subtaskIt = (*headTaskPtr)->getSubtaskList().find(taskIt);
                if(subtaskIt != (*headTaskPtr)->getSubtaskList().end())
                    (*headTaskPtr)->getSubtaskList().erase(subtaskIt);
            }
        }
        TM_TaskIt deleteTask = *taskIt;
        TM_Task* taskPtr = *deleteTask;
        this->dateSortedTasks.erase(taskIt);
        this->databaseSortedTasks.erase(deleteTask);
        delete taskPtr;
    }
}

void TM_TaskManager::deleteCurrentTask()
{
    if(this->currentTask != this->dateSortedTasks.end())
        deleteTask(this->currentTask);
    this->currentTask = this->dateSortedTasks.end();
}

void TM_TaskManager::setCurrentTask(TM_TaskItIt currentTask)
{
    this->currentTask = currentTask;
    if(this->outputPtr!=NULL)
        (*(this->outputPtr))->setExistence(true);
}

void TM_TaskManager::setDateTime(TM_YMD startDate, TM_Time startTime, TM_YMD endDate, TM_Time endTime)
{
    if(!this->getCurrentTask() || (this->getCurrentTask()->getStartDate()==this->getCurrentTask()->getEndDate()&&this->getCurrentTask()->getEndTime()<startTime))
        return;
    TM_TaskIt taskIt = *this->currentTask;
    TM_Task* taskPtr = *taskIt;

    TM_Task* headTaskPtr;
    if(taskPtr->getHeadTaskID() != -1)
    {
        headTaskPtr = *this->databaseSortedTasks.find(new TM_Task("", ZeroDate, ZeroDate, {0,0},{0,0},taskPtr->getHeadTaskID()));
        TM_SubtaskIt subtaskIt;
        if((subtaskIt = headTaskPtr->getSubtaskList().find(this->currentTask)) != headTaskPtr->getSubtaskList().end())
        {
            headTaskPtr->getSubtaskList().erase(subtaskIt);
        }
    }
    this->dateSortedTasks.erase(this->currentTask);

    if(startDate != ZeroDate)
        taskPtr->setStartDate(startDate);
    if(startTime != ZeroTime)
        taskPtr->setStartTime(startTime);
    if(endDate != ZeroDate)
        taskPtr->setEndDate(endDate);
    if(endTime != ZeroTime)
        taskPtr->setEndTime(endTime);

    if(this->storageManPtr&&*this->storageManPtr!=NULL)
        (*this->storageManPtr)->AlterDBTask(taskPtr);

    this->currentTask = this->dateSortedTasks.insert(taskIt);

    if(taskPtr->getHeadTaskID() != -1)
        headTaskPtr->getSubtaskList().insert(this->currentTask);
}

void TM_TaskManager::setTaskName(std::string taskName)
{
    this->getCurrentTask()->setName(taskName);
    if(this->storageManPtr&&*this->storageManPtr!=NULL)
        (*this->storageManPtr)->AlterDBTask(this->getCurrentTask());
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
    int lastMonthIndex = 0, lastYear = -1;
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
                    this->addTask(new TM_Task(name, day1, day2, {0,0}, {24,0},i));
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

TM_TaskItIt TM_TaskManager::getStartIt()
{
    return this->dateSortedTasks.begin();
}

TM_TaskItIt TM_TaskManager::getEndIt()
{
    return this->dateSortedTasks.end();
}

TM_TaskItIt TM_TaskManager::getTaskByID(int id)
{
    TM_TaskIt headIt = this->databaseSortedTasks.find(new TM_Task("", ZeroDate, ZeroDate, {0,0},{0,0},id));
    TM_TaskItIt headItIt = this->dateSortedTasks.end();
    if(headIt != this->databaseSortedTasks.end())
        headItIt = this->dateSortedTasks.find(headIt);
    return headItIt;
}