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

#include <bits/chrono.h>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>

#include "day.hpp"
#include "month.hpp"
#include "parser.hpp"

using std::format;
using std::invalid_argument;
using std::make_unique;
using std::ostringstream;
using std::setfill;
using std::setw;
using std::stoi;
using std::string;
using std::to_string;
using std::unique_ptr;
using std::chrono::days;
using std::chrono::floor;
using std::chrono::hh_mm_ss;
using std::chrono::seconds;
using std::chrono::system_clock;
using std::chrono::year_month_day;

Parser::Parser() = default;

auto Parser::ParseFormat(const string& argument, const ClockInterface& clock) -> string
{
    string formattedDate;        // The output date being constructed
    string tempDate;             // A temporary string in case it's needed
    int tempHour = 0;            // Stores the hour temporarily for format calculations
    hh_mm_ss offset{seconds{0}}; // Stores the UTC offset as hours, minutes, and seconds.

    constexpr int MIDDAY   = 12; // Defines the hour at which PM begins
    constexpr int HALF_DAY = 12; // Defines the number of hours in half a day.

    for (int i = 0; i < argument.size(); i++)
    {
        if (argument.at(i) == '%' && i + 1 < argument.size())
        {
            switch (argument.at(i + 1))
            {
            case 'a':
                formattedDate += getShortDayName(clock.getDayOfTheWeek());
                i++;
                break;
            case 'A':
                formattedDate += getLongDayName(clock.getDayOfTheWeek());
                i++;
                break;
            case 'b':
                formattedDate += getShortMonthName(clock.getMonth());
                i++;
                break;
            case 'B':
                formattedDate += getLongMonthName(clock.getMonth());
                i++;
                break;
            case 'd':
                formattedDate += formatTwoDigits(clock.getDay());
                i++;
                break;
            case 'e':
                formattedDate += to_string(clock.getDay());
                i++;
                break;
            case 'H':
                formattedDate += formatTwoDigits(clock.getHour());
                i++;
                break;
            case 'I':
                tempHour = clock.getHour();
                if (tempHour > MIDDAY)
                {
                    formattedDate = formatTwoDigits(tempHour - HALF_DAY);
                }
                else
                {
                    formattedDate = formatTwoDigits(tempHour);
                }
                i++;
                break;
            case 'j':
                formattedDate += formatThreeDigits(clock.getDayOfTheYear() + 1);
                i++;
                break;
            case 'm':
                formattedDate += formatTwoDigits(clock.getMonth());
                i++;
                break;
            case 'M':
                formattedDate += formatTwoDigits(clock.getMin());
                i++;
                break;
            case 'p':
                tempHour = clock.getHour();
                if (tempHour <= MIDDAY)
                {
                    formattedDate += "AM";
                }
                else
                {
                    formattedDate += "PM";
                }
                i++;
                break;
            case 'r':
                tempHour = clock.getHour();
                if (tempHour > MIDDAY)
                {
                    formattedDate = formatTwoDigits(tempHour - HALF_DAY);
                }
                else
                {
                    formattedDate = formatTwoDigits(tempHour);
                }
                formattedDate += ':';
                formattedDate += formatTwoDigits(clock.getMin());
                formattedDate += ':';
                formattedDate += formatTwoDigits(clock.getSec());
                formattedDate += ' ';
                tempHour = clock.getHour();
                if (tempHour <= MIDDAY)
                {
                    formattedDate += "AM";
                }
                else
                {
                    formattedDate += "PM";
                }
                i++;
                break;
            case 'R':
                formattedDate += formatTwoDigits(clock.getHour());
                formattedDate += ':';
                formattedDate += formatTwoDigits(clock.getMin());
                i++;
                break;
            case 'S':
                formattedDate += formatTwoDigits(clock.getSec());
                i++;
                break;
            case 'T':
                formattedDate += formatTwoDigits(clock.getHour());
                formattedDate += ':';
                formattedDate += formatTwoDigits(clock.getMin());
                formattedDate += ':';
                formattedDate += formatTwoDigits(clock.getSec());
                i++;
                break;
            case 'u':
                formattedDate += to_string(clock.getDayOfTheWeek());
                if (formattedDate == "0")
                {
                    formattedDate = "7";
                }
                i++;
                break;
            case 'w':
                formattedDate += to_string(clock.getDayOfTheWeek());
                i++;
                break;
            case 'y':
                tempDate = to_string(clock.getYear());
                formattedDate += tempDate.substr(tempDate.size() - 2);
                i++;
                break;
            case 'Y':
                formattedDate += to_string(clock.getYear());
                i++;
                break;
            case 'z':
                offset = hh_mm_ss{clock.getUtcOffset()};
                if (offset.is_negative())
                {
                    formattedDate += '-';
                }
                else
                {
                    formattedDate += '+';
                }
                formattedDate += format("{:02}", offset.hours().count());
                formattedDate += format("{:02}", offset.minutes().count());
                i++;
                break;
            case 'Z':
                formattedDate += clock.getTimeZone();
                i++;
                break;
            case '%':
                formattedDate += '%';
                i++;
                break;
            default:
                formattedDate += argument.at(i);
                break;
            }
        }
        else if (argument.at(i) != '+')
        {
            formattedDate += argument.at(i);
        }
    }

    formattedDate += '\n';

    return formattedDate;
}

auto Parser::ParseDate(string& argument) -> unique_ptr<timespec>
{
    string monthStr;
    string dayStr;
    string hourStr;
    string minuteStr;
    string yearStr;
    string secondStr;
    int month                          = 0;
    int day                            = 0;
    int hour                           = 0;
    int minute                         = 0;
    int year                           = 0;
    int second                         = 0;
    constexpr int MAX_MONTH            = 12;
    constexpr int MAX_DAY              = 31;
    constexpr int MAX_HOUR             = 24;
    constexpr int MAX_MIN              = 60;
    constexpr int MAX_SEC              = 60;
    constexpr int BASE_YEAR            = 1900;
    constexpr int BASE_LENGTH          = 7;
    constexpr int MAX_LENGTH           = 14;
    constexpr int MONTH_POS            = 0;
    constexpr int DAY_POS              = 2;
    constexpr int HOUR_POS             = 4;
    constexpr int MIN_POS              = 6;
    constexpr int YEAR_POS             = 8;
    constexpr int POINT_POS            = 12;
    constexpr int SECOND_POS           = 13;
    constexpr int DECEMBER             = 12;
    constexpr int TWO_DIGIT_YEAR_PIVOT = 69; // First two-digit year read as 19yy (1969), anything below it is read as 20yy
    tm localtime                       = {};
    time_t time                        = 0;
    unique_ptr<timespec> convertedTime = make_unique<timespec>();

    if (argument.size() < BASE_LENGTH)
    {
        return nullptr;
    }

    // Extracts the various sections of the date from the argument (ie. day, month, etc)
    monthStr += argument.substr(MONTH_POS, 2);

    if (monthStr.find_first_not_of("0123456789") != string::npos)
    {
        return nullptr;
    }

    if (stoi(monthStr) > DECEMBER)
    {
        return nullptr;
    }

    dayStr += argument.substr(DAY_POS, 2);
    hourStr += argument.substr(HOUR_POS, 2);
    minuteStr += argument.substr(MIN_POS, 2);

    if (dayStr.find_first_not_of("0123456789") != string::npos)
    {
        return nullptr;
    }

    if (hourStr.find_first_not_of("0123456789") != string::npos)
    {
        return nullptr;
    }

    if (minuteStr.find_first_not_of("0123456789") != string::npos)
    {
        return nullptr;
    }

    if (argument.size() > BASE_LENGTH) // If the argument is longer than 7, it means we want to set the year
    {
        yearStr += argument.substr(YEAR_POS, 4);

        if (yearStr.find_first_not_of("0123456789") != string::npos)
        {
            return nullptr;
        }

        // Only the last two digits of the year were given: infers the century as POSIX requires
        // (00 to 68 give 2000 to 2068, 69 to 99 give 1969 to 1999)
        if (yearStr.size() == 2)
        {
            if (stoi(yearStr) < TWO_DIGIT_YEAR_PIVOT)
            {
                yearStr = "20" + yearStr;
            }
            else
            {
                yearStr = "19" + yearStr;
            }
        }

        if (yearStr.empty()) // if the year is empty, we assume the current year
        {
            auto currentYear = year_month_day{floor<days>(system_clock::now())}.year();
            yearStr          = to_string(static_cast<int>(currentYear));
        }
    }

    if (argument.size() > MAX_LENGTH && argument.at(POINT_POS) == '.') // If the argument is longer than 12 and the char at position 12 is '.', it means we want to set the seconds
    {
        secondStr += argument.substr(SECOND_POS, 2);

        if (secondStr.find_first_not_of("0123456789") != string::npos)
        {
            return nullptr;
        }
    }

    // Converts the strings to integer
    month  = stoi(monthStr);
    day    = stoi(dayStr);
    hour   = stoi(hourStr);
    minute = stoi(minuteStr);

    try
    {
        year = stoi(yearStr);
    }
    catch (const invalid_argument& e)
    {
        year = 0;
    }

    try
    {
        second = stoi(secondStr);
    }
    catch (const invalid_argument& e)
    {
        second = 0;
    }

    if (month > 0 && month <= MAX_MONTH)
    {
        localtime.tm_mon = month - 1;
    }

    if (day > 0 && day <= MAX_DAY)
    {
        localtime.tm_mday = day;
    }

    if (hour > 0 && hour <= MAX_HOUR)
    {
        localtime.tm_hour = hour;
    }

    if (minute > 0 && minute <= MAX_MIN)
    {
        localtime.tm_min = minute;
    }

    if (second > 0 && second <= MAX_SEC)
    {
        localtime.tm_sec = second;
    }

    localtime.tm_year = year - BASE_YEAR;

    // Sets isdst to -1 so that mktime determines if the time is dst or not
    localtime.tm_isdst = -1;

    time = mktime(&localtime);

    convertedTime->tv_sec  = time;
    convertedTime->tv_nsec = 0;

    return convertedTime;
}

auto Parser::formatTwoDigits(int value) -> string
{
    ostringstream oss;
    oss << setw(2) << setfill('0') << value;
    return oss.str();
}

auto Parser::formatThreeDigits(int value) -> string
{
    ostringstream oss;
    oss << setw(3) << setfill('0') << value;
    return oss.str();
}
