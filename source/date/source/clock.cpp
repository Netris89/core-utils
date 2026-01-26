/*
 *  Copyright (c) 2025, Ludovic Hansen
 *  License: MIT
 *
 *  Description:
 *  This application implements a full version of the `date` command in C++,
 *  conforming to the POSIX specification. It displays the current date and time,
 *  supports timezone adjustments, allows custom output formats, and permits
 *  setting the system date and time when authorized.
 *
 *  Usage: ./date [-u] [+format] [mmddhhmm[[cc]yy][.ss]]
 *
 *  Supported options:
 *    -u      : Display the date and time in "UTC0".
 *
 *  Supported features:
 *    - Display the current date and time in local time or UTC (-u).
 *    - Custom formatting of output using the +format option.
 *    - Setting the system date and time (requires appropriate privileges).
 *    - Handling of the TZ environment variable for timezone settings.
 *
 *  Note: More details on the `date` command and its behavior can be found here:
 *  https://pubs.opengroup.org/onlinepubs/9799919799/utilities/date.html
 */

#include <chrono>
#include <cstdlib>
#include <ctime>
#include <format>
#include <string>

#include <sys/select.h>

#include "clock.hpp"
#include "day.hpp"
#include "month.hpp"

using duration = std::chrono::system_clock::duration;
using std::chrono::current_zone;
using std::chrono::system_clock;
using std::chrono::zoned_time;
using std::exit;
using std::string;

Clock::Clock() : now(0), time(nullptr), day(Day::Monday), month(Month::January) {}

Clock::Clock(bool isUtc)
{
    now = system_clock::to_time_t(system_clock::now());

    if (isUtc)
    {
        time     = gmtime(&now);
        timeZone = "UTC";
    }
    else
    {
        time = localtime(&now);
        zoned_time<duration> zonedTime(current_zone(), system_clock::from_time_t(now));
        timeZone = std::format("{:%Z}", zonedTime);
    }

    day   = static_cast<Day>(time->tm_wday);
    month = static_cast<Month>(time->tm_mon);
}

auto Clock::getYear() const -> int
{
    return TM_YEAR_BASE + time->tm_year;
}

auto Clock::getMonth() const -> int
{
    return time->tm_mon;
}

auto Clock::getDay() const -> int
{
    return time->tm_mday;
}

auto Clock::getHour() const -> int
{
    return time->tm_hour;
}

auto Clock::getMin() const -> int
{
    return time->tm_min;
}

auto Clock::getSec() const -> int
{
    return time->tm_sec;
}

auto Clock::getDayOfTheWeek() const -> int
{
    return time->tm_wday;
}

auto Clock::getTimeZone() const -> string
{
    return timeZone;
}

void Clock::setTime(timespec* newTime)
{
    clock_settime(CLOCK_REALTIME, newTime);
    exit(EXIT_SUCCESS);
}
