#pragma once

#include <chrono>
#include <iostream>
#include <string>

static std::string monthNames[12] = {"January","February","March","April","May","June","July","August","September","October","November","December"};
static std::string dayNames[7] = {"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};

struct TM_Time { int hours, minutes; bool operator<(const TM_Time& b) const { return hours<b.hours || (hours==b.hours&&minutes<b.minutes); } };

int TM_TimeMinutes(TM_Time time);

int weekDayFromDate(std::chrono::year_month_day ymd);

std::string TM_DateToString(std::chrono::year_month_day ymd);

std::chrono::year_month_day getCurrentDate();

unsigned TM_NumMonthDays(std::chrono::year_month_day ymd);