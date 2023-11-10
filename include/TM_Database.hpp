#pragma once

#include <iostream>

#include <sqlite3.h>

class TM_StorageManager 
{
    public:
        TM_StorageManager(std::string dbFilePath);
    private:
        sqlite3* dbPtr;
};