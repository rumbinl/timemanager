#include <TM_Database.hpp>

TM_StorageManager::TM_StorageManager(std::string dbFilePath)
{
    sqlite3_open_v2(dbFilePath.c_str(), &this->dbPtr, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);

    int errorCode;

    if((errorCode = sqlite3_exec(this->dbPtr, "CREATE TABLE IF NOT EXISTS TimeSections (Name varchar(255), startDatetime DATETIME, endDatetime DATETIME, headTaskID INTEGER, repeat INTEGER, ID INTEGER PRIMARY KEY AUTOINCREMENT);", NULL, NULL, NULL)) != SQLITE_OK)
        std::cout<<sqlite3_errmsg(this->dbPtr)<<std::endl;
}

void TM_StorageManager::CreateDBTask(TM_Task* taskPtr)
{
    std::string queryString = "INSERT INTO TimeSections (Name, startDatetime, endDatetime, headTaskID, repeat) VALUES ('"+taskPtr->getName()+"', '"+TM_GetDateTimeString(taskPtr->getStartDate(), taskPtr->getStartTime())+"', '"+TM_GetDateTimeString(taskPtr->getEndDate(), taskPtr->getEndTime())+"',"+std::to_string(taskPtr->getHeadTaskID())+", 0);";
    int errorCode;
    if((errorCode = sqlite3_exec(this->dbPtr, queryString.c_str(), NULL, NULL, NULL)) != SQLITE_OK)
        std::cout<<sqlite3_errmsg(this->dbPtr)<<std::endl;
    else
        taskPtr->setDBID(sqlite3_last_insert_rowid(this->dbPtr));
}

void TM_StorageManager::AlterDBTask(TM_Task* taskPtr)
{
    std::string queryString = "UPDATE TimeSections SET Name='"+taskPtr->getName()+"', startDatetime='"+TM_GetDateTimeString(taskPtr->getStartDate(), taskPtr->getStartTime())+"', endDatetime='"+TM_GetDateTimeString(taskPtr->getEndDate(), taskPtr->getEndTime())+"', headTaskID="+std::to_string(taskPtr->getHeadTaskID())+", repeat="+std::to_string(taskPtr->getRepeat())+" WHERE ID="+std::to_string(taskPtr->getDBID())+";";
    int errorCode;
    if((errorCode = sqlite3_exec(this->dbPtr, queryString.c_str(), NULL, NULL, NULL)) != SQLITE_OK)
        std::cout<<sqlite3_errmsg(this->dbPtr)<<std::endl;
}

void TM_StorageManager::DeleteDBTask(TM_Task* taskPtr)
{
    std::string queryString = "DELETE FROM TimeSections WHERE ID="+std::to_string(taskPtr->getDBID())+";";
    int errorCode;
    if((errorCode = sqlite3_exec(this->dbPtr, queryString.c_str(), NULL, NULL, NULL)) != SQLITE_OK)
        std::cout<<sqlite3_errmsg(this->dbPtr)<<std::endl;
}

void TM_StorageManager::LoadTasks(TM_TaskManager* taskManPtr)
{
    std::string queryString = "SELECT * FROM TimeSections ORDER BY startDatetime ASC;";
    int errorCode;
    if((errorCode = sqlite3_exec(this->dbPtr, queryString.c_str(), [](void* x, int columnCount, char** columnData, char** columnNames) -> int {

        TM_TaskManager* taskManPtr = (TM_TaskManager*)x;

        int d,m,y, hr, mn;
        sscanf(columnData[STARTDATE_COLUMN_IDX], "%d-%d-%d %d:%d", &d, &m, &y, &hr, &mn);
        std::chrono::year_month_day startDate = getDateFromVars(d,m,y);
        TM_Time startTime = {hr, mn};
        sscanf(columnData[ENDDATE_COLUMN_IDX], "%d-%d-%d %d:%d", &d, &m, &y, &hr, &mn);
        std::chrono::year_month_day endDate = getDateFromVars(d,m,y);
        TM_Time endTime = {hr, mn};

        taskManPtr->addTask(new TM_Task(columnData[NAME_COLUMN_IDX], startDate, endDate, startTime, endTime, std::atoi(columnData[ID_COLUMN_IDX]), std::atoi(columnData[HEADID_COLUMN_IDX]), std::atoi(columnData[REPEAT_COLUMN_IDX])));

        return 0;
    }, taskManPtr, NULL)) != SQLITE_OK)
        std::cout<<sqlite3_errmsg(this->dbPtr)<<std::endl;
    taskManPtr->initializeSubtasks();
}

TM_StorageManager::~TM_StorageManager()
{
    sqlite3_close(this->dbPtr);
}