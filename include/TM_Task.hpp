#pragma once

#include <iostream>
#include <vector>

class TM_Task 
{
    public:
        TM_Task(std::string name, /* date-time variable, */std::vector<TM_Task> sub_tasks={});
        void changeDate(/*date-time variable*/);
        void changeName(std::string name);
        void rescheduleSubtasks();
        ~TM_Task();
    private:
        std::string name; // once it has subtasks the date time variable automatically becomes the deadline for all subtasks
        // date time data variable 
        TM_Task* rootTask;
        std::vector<TM_Task> sub_tasks();
};

struct TM_TaskComp
{
        bool operator()(TM_Task* a, TM_Task* b);
};