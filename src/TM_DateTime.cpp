#include <TM_DateTime.hpp>

int TM_TimeMinutes(TM_Time time)
{
    return time.hours * 60 + time.minutes;
}

int weekDayFromDate(std::chrono::year_month_day ymd)
{
    return (std::chrono::weekday{std::chrono::sys_days(ymd)}).c_encoding();
}

std::chrono::year_month_day getCurrentDate()
{
    std::chrono::time_point now{std::chrono::system_clock::now()};
    return std::chrono::year_month_day{std::chrono::floor<std::chrono::days>(now)};
}

unsigned TM_NumMonthDays(std::chrono::year_month ym)
{
    return static_cast<unsigned>(std::chrono::year_month_day_last{ym.year(),std::chrono::month_day_last{ym.month()/std::chrono::last}}.day());
}