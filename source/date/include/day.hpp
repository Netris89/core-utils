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

#include <array>
#include <cstdint>
#include <string_view>

/**
 * @brief Represents the days of the week.
 *
 * The Day enum class defines the days from Sunday (0) to Saturday (6).
 */
enum class Day : std::uint8_t
{
    Sunday    = 0,
    Monday    = 1,
    Tuesday   = 2,
    Wednesday = 3,
    Thursday  = 4,
    Friday    = 5,
    Saturday  = 6
};

/**
 * @brief Returns the abbreviated (short) name of a day of the week.
 *
 * @param day An integer representing the day of the week (0 = Monday, 6 = Sunday).
 * @return A string_view containing the 3-letter abbreviation of the day.
 *
 * @throws std::out_of_range if day is not in the range [0, 6].
 */
inline auto getShortDayName(int day) -> std::string_view
{
    static constexpr std::array<std::string_view, 7> shortNames = {
        "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

    return shortNames.at(day);
}

/**
 * @brief Returns the full (long) name of a day of the week.
 *
 * @param day An integer representing the day of the week (0 = Monday, 6 = Sunday).
 * @return A string_view containing the full name of the day.
 *
 * @throws std::out_of_range if day is not in the range [0, 6].
 */
inline auto getLongDayName(int day) -> std::string_view
{
    static constexpr std::array<std::string_view, 7> longNames = {
        "Sunday", "Monday", "Tuesday",
        "Wednesday", "Thursday", "Friday", "Saturday"};

    return longNames.at(day);
}
