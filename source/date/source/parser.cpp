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

#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include "day.hpp"
#include "month.hpp"
#include "parser.hpp"

using std::cout;
using std::make_unique;
using std::ostringstream;
using std::setfill;
using std::setw;
using std::stoi;
using std::string;
using std::to_string;
using std::unique_ptr;

Parser::Parser() = default;

auto Parser::ParseFormat(const string& argument, const ClockInterface& clock) -> string
{
    string formattedDate; // The output date being constructed
    string tempDate;      // A temporary string in case it's needed

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
                // TODO() : Hour 12h (01-12)
                break;
            case 'j':
                // TODO() : Day of the year (001-366)
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
                // TODO() : Handles AM/PM
                break;
            case 'r':
                // TODO(): 12h format (%I:%M:%S %p)
            case 'R':
                formattedDate += formatTwoDigits(clock.getHour());
                formattedDate += ":";
                formattedDate += formatTwoDigits(clock.getMin());
                i++;
                break;
            case 'S':
                formattedDate += formatTwoDigits(clock.getSec());
                i++;
                break;
            case 'T':
                formattedDate += formatTwoDigits(clock.getHour());
                formattedDate += ":";
                formattedDate += formatTwoDigits(clock.getMin());
                formattedDate += ":";
                formattedDate += formatTwoDigits(clock.getSec());
                i++;
                break;
            case 'u':
                // TODO() : Day of the week (1=Monday, 7=Sunday)
                break;
            case 'w':
                // TODO() : Day of the week (0=Sunday, 6=Saturday)
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
                // TODO() : UTC offset
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
    string centuryStr;
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
    tm localtime                       = {};
    time_t time                        = 0;
    unique_ptr<timespec> convertedTime = make_unique<timespec>();

    if (argument.size() < BASE_LENGTH)
    {
        cout << "Invalid argument\n";
        return nullptr;
    }

    // Extracts the various sections of the date from the argument (ie. day, month, etc)
    monthStr += argument.substr(MONTH_POS, 2);
    dayStr += argument.substr(DAY_POS, 2);
    hourStr += argument.substr(HOUR_POS, 2);
    minuteStr += argument.substr(MIN_POS, 2);

    if (argument.size() > BASE_LENGTH) // If the argument is longer than 7, it means we want to set the year
    {
        yearStr += argument.substr(YEAR_POS, 4);
    }

    if (argument.size() > MAX_LENGTH && argument.at(POINT_POS) == '.') // If the argument is longer than 12 and the char at position 12 is '.', it means we want to set the seconds
    {
        secondStr += argument.substr(SECOND_POS, 2);
    }

    // Converts the strings to integer
    month  = stoi(monthStr);
    day    = stoi(dayStr);
    hour   = stoi(hourStr);
    minute = stoi(minuteStr);
    year   = stoi(yearStr);
    second = stoi(secondStr);

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
