#include <TM_DateTime.hpp>

std::chrono::year_month_day TM_YMD(int d, int m, int y)
{
    std::chrono::year_month_day ymd{std::chrono::year(y),std::chrono::month(m),std::chrono::day(d)};
    return ymd;
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