#pragma once

#include <iostream>
#include <sstream>
#include <chrono>

#include <sqlite3.h>

#include <TM_Task.hpp>
#include <TM_TaskManager.hpp>

#define NAME_COLUMN_IDX 0
#define STARTDATE_COLUMN_IDX 1
#define ENDDATE_COLUMN_IDX 2
#define ID_COLUMN_IDX 3

class TM_StorageManager 
{
    public:
        TM_StorageManager(std::string dbFilePath);
        void CreateDBTask(TM_Task* taskPtr);
        void AlterDBTask(TM_Task* taskPtr);
        void DeleteDBTask(TM_Task* taskPtr);
        void LoadTasks(TM_TaskManager* taskManPtr);
        ~TM_StorageManager();
    private:
        sqlite3* dbPtr;
};