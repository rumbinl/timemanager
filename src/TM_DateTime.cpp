#include <TM_DateTime.hpp>

int TM_TimeMinutes(TM_Time time)
{
    return time.hours * 60 + time.minutes;
}

std::chrono::days TM_GetTimeDays(TM_Time time)
{
    return std::chrono::days{(unsigned)(time.hours)/24};
}

std::chrono::year_month_day getDateFromVars(int d, int m, int y)
{
    return std::chrono::year_month_day{std::chrono::year{(int)y}, std::chrono::month{(unsigned)m}, std::chrono::day{(unsigned)d}};
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

std::string TM_DateToString(std::chrono::year_month_day ymd)
{
    return std::to_string(unsigned(ymd.day())) + " " + monthNames[unsigned(ymd.month())-1] + " " + std::to_string(int(ymd.year()));
}

unsigned TM_NumMonthDays(std::chrono::year_month_day ymd)
{
    return static_cast<unsigned>(std::chrono::year_month_day_last{ymd.year(),std::chrono::month_day_last{ymd.month()/std::chrono::last}}.day());
}

std::string TM_GetTimeString(TM_Time time)
{
    return (time.hours<10?"0":"")+std::to_string(time.hours)+':'+(time.minutes<10?"0":"")+std::to_string(time.minutes)+":"+(time.seconds<10?"0":"")+std::to_string(time.seconds);
}

std::string TM_GetDateString(std::chrono::year_month_day ymd)
{
    return std::to_string((unsigned)ymd.day())+'-'+std::to_string((unsigned)ymd.month())+'-'+std::to_string((int)ymd.year());
}

std::string TM_GetDateTimeString(std::chrono::year_month_day ymd, TM_Time time)
{
    return TM_GetDateString(ymd)+' '+TM_GetTimeString(time);
}

std::chrono::days TM_GetTimeDateOverflow(TM_Time time)
{
    return std::chrono::days{(unsigned)time.hours/24}; 
}

TM_Time TM_NormalizeTime(TM_Time time)
{
    return {time.hours%24, time.minutes};
}

TM_Time TM_GetTimeFromSeconds(int seconds)
{
    return {seconds/3600, (seconds%3600)/60, seconds%60};
}