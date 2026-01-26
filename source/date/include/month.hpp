/*
 *  Copyright (c) 2025, Ludovic Hansen
 *  License: MIT
 *
 *  Description:
 *  This application implements a full version of the `date` command in C++,
 *  conforming to the POSIX specification. It displays the current date and
 * time, supports timezone adjustments, allows custom output formats, and
 * permits setting the system date and time when authorized.
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
 * @brief Represents the month of the year.
 *
 * The Month enum class defines the month from January (0) to December (6).
 */
enum class Month : std::uint8_t
{
    January   = 0,
    February  = 1,
    March     = 2,
    April     = 3,
    May       = 4,
    June      = 5,
    July      = 6,
    August    = 7,
    September = 8,
    October   = 9,
    November  = 10,
    December  = 11
};

/**
 * @brief Returns the abbreviated (short) name of a month.
 *
 * @param month An integer representing the month (0 = January, 11 = December).
 * @return A string_view containing the 3-letter abbreviation of the month.
 *
 * @throws std::out_of_range if month is not in the range [0, 11].
 */
inline auto getShortMonthName(int month) -> std::string_view
{
    static constexpr std::array<std::string_view, 12> shortNames = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

    return shortNames.at(month);
}

/**
 * @brief Returns the full (long) name of a month.
 *
 * @param month An integer representing the month (0 = January, 11 = December).
 * @return A string_view containing the full name of the month.
 *
 * @throws std::out_of_range if month is not in the range [0, 11].
 */
inline auto getLongMonthName(int month) -> std::string_view
{
    static constexpr std::array<std::string_view, 12> longNames = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"};

    return longNames.at(month);
}
