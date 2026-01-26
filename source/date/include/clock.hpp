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

#pragma once

#include <ctime>

#include "clockInterface.hpp"
#include "day.hpp"
#include "month.hpp"

/**
 * @class Clock
 * @brief Provides access to the current date and time, in local time or UTC.
 *
 * The Clock class wraps standard C++ time functions and offers an interface to retrieve
 * specific time and date components, including timezone information, day name, and date formatting.
 * It supports initialization in either local time or UTC.
 */
class Clock : public ClockInterface
{
private:
    time_t now;                               // Stores the current time as time_t
    tm* time;                                 // Pointer to a tm structure that will hold broken-down time
    Day day;                                  // Enum value representing a specific day, initialized to default (0)
    Month month;                              // Enum value representing a specific month, initialized to default (0)
    static constexpr int TM_YEAR_BASE = 1900; // Base for calculating date
    std::string timeZone;                     // String representing the timezone

public:
    /**
     * @brief Default constructor. Initializes members to default values.
     */
    Clock();

    /**
     * @brief Constructs the Clock object using either local time or UTC.
     *
     * @param isUtc If true, initializes the clock to UTC time; otherwise, uses local time.
     */
    explicit Clock(bool isUtc);

    /**
     * @brief Returns the full year (e.g., 2025).
     * @return Current year as an integer.
     */
    auto getYear() const -> int override;

    /**
     * @brief Returns the current month as an integer (0 = January, 11 = December).
     * @return Integer representing the current month.
     */
    auto getMonth() const -> int override;

    /**
     * @brief Returns the current day of the month (1–31).
     * @return Day of the month.
     */
    auto getDay() const -> int override;

    /**
     * @brief Returns the current hour (0–23).
     * @return Hour of the day.
     */
    auto getHour() const -> int override;

    /**
     * @brief Returns the current minute (0–59).
     * @return Minute of the hour.
     */
    auto getMin() const -> int override;

    /**
     * @brief Returns the current second (0–59).
     * @return Second of the minute.
     */
    auto getSec() const -> int override;

    /**
     * @brief Returns the current day of the week as an integer (0 = Sunday, 6 = Saturday).
     * @return Integer representing the day of the week.
     */
    auto getDayOfTheWeek() const -> int override;

    /**
     * @brief Returns the current timezone abbreviation as a string.
     * @return Timezone string (e.g., "UTC", "CEST").
     */
    auto getTimeZone() const -> std::string override;

    /**
     * @brief Sets the system's real-time clock to a new time.
     *
     * This method uses the POSIX `clock_settime` function to set the system clock
     * (`CLOCK_REALTIME`) to the specified time. The time provided must be in the
     * `timespec` format, which includes seconds and nanoseconds.
     *
     * @param newTime A pointer to a `timespec` structure representing the new time
     *                to be set for the system clock.
     *
     * @note This function requires appropriate privileges (typically root) to
     *       successfully set the system time.
     *
     * @see clock_settime
     */
    static void setTime(timespec*);
};
