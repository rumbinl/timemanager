#include <TM_Database.hpp>

TM_StorageManager::TM_StorageManager(std::string dbFilePath)
{
    sqlite3_open_v2(dbFilePath.c_str(), &this->dbPtr, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);

    int errorCode;

    if((errorCode = sqlite3_exec(this->dbPtr, "CREATE TABLE IF NOT EXISTS TimeSections (Name varchar(255), startDatetime DATETIME, endDatetime DATETIME, ID INTEGER PRIMARY KEY AUTOINCREMENT);", NULL, NULL, NULL)) != SQLITE_OK)
        std::cout<<sqlite3_errmsg(this->dbPtr)<<std::endl;
}

void TM_StorageManager::CreateDBTask(TM_Task* taskPtr)
{
    std::string queryString = "INSERT INTO TimeSections (Name, startDatetime, endDatetime) VALUES ('"+taskPtr->getName()+"', '"+TM_GetDateTimeString(taskPtr->getStartDate(), taskPtr->getStartTime())+"', '"+TM_GetDateTimeString(taskPtr->getEndDate(), taskPtr->getEndTime())+"');";
    int errorCode;
    if((errorCode = sqlite3_exec(this->dbPtr, queryString.c_str(), [](void* x, int rowCount, char** columnStrings, char** columnNames) -> int { 
        if(rowCount > 0)
        {
            std::cout<<rowCount<<std::endl;
            std::cout<<"ID:"<<columnStrings[ID_COLUMN_IDX]<<std::endl;
        }
        return 0; 
    }, NULL, NULL)) != SQLITE_OK)
        std::cout<<sqlite3_errmsg(this->dbPtr)<<std::endl;
}

void TM_StorageManager::AlterDBTask(TM_Task* taskPtr)
{

}

void TM_StorageManager::DeleteDBTask(TM_Task* taskPtr)
{

}

void TM_StorageManager::LoadTasks(TM_TaskManager* taskManPtr)
{

}

TM_StorageManager::~TM_StorageManager()
{
    sqlite3_close(this->dbPtr);
}