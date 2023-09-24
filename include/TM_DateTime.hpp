#pragma once

#include <chrono>
#include <iostream>

struct TM_Time { int hours, minutes; bool operator<(const TM_Time& b) const { return hours<b.hours || (hours==b.hours&&minutes<b.minutes); } };

int TM_TimeMinutes(TM_Time time);

int weekDayFromDate(std::chrono::year_month_day ymd);

std::chrono::year_month_day getCurrentDate();

unsigned TM_NumMonthDays(std::chrono::year_month ymd);