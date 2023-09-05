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

unsigned TM_NumMonthDays(std::chrono::year_month_day ymd)
{
    return static_cast<unsigned>(std::chrono::year_month_day_last{ymd.year(),std::chrono::month_day_last{ymd.month()/std::chrono::last}}.day());
}