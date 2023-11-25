#pragma once

#include <chrono>
#include <iostream>
#include <string>

typedef std::chrono::year_month_day TM_YMD;

static std::string monthNames[12] = {"January","February","March","April","May","June","July","August","September","October","November","December"};
static std::string dayNames[7] = {"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};

#define ZeroDate (TM_YMD{std::chrono::January/1/0})
#define ZeroTime (TM_Time{-1,-1})

struct TM_Time { int hours, minutes; 

    bool operator==(const TM_Time& b) const { return hours==b.hours && minutes == b.minutes; } 

    bool operator<(const TM_Time& b) const { return hours<b.hours || (hours==b.hours&&minutes<b.minutes); } 

    bool operator>(const TM_Time& b) const { return hours>b.hours || (hours==b.hours&&minutes>b.minutes); } 

    TM_Time operator-(const TM_Time& b) const {
        int res = 60*(hours - b.hours) + minutes-b.minutes;
        return {res/60,res%60};
    }

    TM_Time operator+(const TM_Time& b) const {
        int res = 60*(hours+b.hours) + minutes+b.minutes;
        return {res/60,res%60};
    }
};

struct TM_TimeRange {
    TM_YMD startDate, endDate;
    TM_Time startTime, endTime;

    bool operator==(const TM_TimeRange& b) const
    {
        return b.startDate == startDate && b.endDate == endDate && b.startTime == startTime && b.endTime == endTime;
    }

    bool operator<(const TM_TimeRange& b) const 
    { 
        if(startDate == b.startDate) 
        {
            if(startTime == b.startTime) 
            {
                if(endDate == b.endDate)

                    return endTime < b.endTime;

                return endDate < b.endDate;
            }
            return startTime < b.startTime;
        }
        return startDate < b.startDate;
    }
};

int TM_TimeMinutes(TM_Time time);

int weekDayFromDate(std::chrono::year_month_day ymd);

std::string TM_DateToString(std::chrono::year_month_day ymd);

std::chrono::year_month_day getDateFromVars(int d, int m, int y);

std::chrono::year_month_day getCurrentDate();

unsigned TM_NumMonthDays(std::chrono::year_month_day ymd);

std::string TM_GetTimeString(TM_Time time);

std::string TM_GetDateString(std::chrono::year_month_day ymd);

std::string TM_GetDateTimeString(std::chrono::year_month_day ymd, TM_Time time);

std::chrono::days TM_GetTimeDateOverflow(TM_Time time);

TM_Time TM_NormalizeTime(TM_Time time);