#pragma once

#include <chrono>
#include <iostream>

std::chrono::year_month_day TM_YMD(int d, int m, int y);

int weekDayFromDate(std::chrono::year_month_day ymd);

unsigned TM_NumMonthDays(std::chrono::year_month ymd);