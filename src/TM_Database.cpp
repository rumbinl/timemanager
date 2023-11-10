#include <TM_Database.hpp>

TM_StorageManager::TM_StorageManager(std::string dbFilePath)
{
    sqlite3_open_v2(dbFilePath.c_str(), &this->dbPtr, NULL, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);

    int errorCode;

    if((errorCode = sqlite3_exec("CREATE TABLE IF NOT EXISTS TimeSections (Name varchar(255), startDatetime DATETIME, endDatetime DATETIME, ID INTEGER PRIMARY KEY AUTOINCREMENT);")) != SQLITE_OK)
        std::cout<<sqlite3_errmsg(this->dbPtr)<<std::endl;
}