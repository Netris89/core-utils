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
#include <memory>
#include <string>

#include "clockInterface.hpp"

/**
 * @class Parser
 * @brief Parses date format strings and returns formatted date strings.
 *
 * This class interprets custom date formatting directives (e.g., %Y, %H, %M)
 * and outputs a formatted date string using information from a Clock object.
 */
class Parser
{
private:
    /**
     * @brief Formats an integer as a two-digit string with leading zeros if necessary.
     *
     * @param value The integer value to format.
     * @return A string representing the integer, padded with leading zeros to ensure at least two digits.
     *
     * For example, an input of 5 will return "05", and an input of 23 will return "23".
     */
    static auto formatTwoDigits(int value) -> std::string;

public:
    /**
     * @brief Default constructor.
     *
     * Constructs a Parser with default-initialized Clock.
     */
    Parser();

    /**
     * @brief Parses a format string and returns the formatted date string.
     *
     * The format string can contain date/time specifiers such as:
     * - `%a` : Abbreviated weekday name
     * - `%b` : Abbreviated month name
     * - `%e` : Day of the month
     * - `%H` : Hour (00–23)
     * - `%M` : Minute (00–59)
     * - `%S` : Second (00–59)
     * - `%Z` : Timezone abbreviation
     * - `%Y` : Full year (e.g., 2025)
     *
     * Characters not preceded by `%` are passed through unchanged, except `+` which is ignored.
     *
     * @param argument The input format string to parse and transform.
     * @return A formatted date string based on the provided format.
     */
    static auto ParseFormat(const std::string&, const ClockInterface&) -> std::string;

    /**
     * @brief Parses a date string into a timespec structure.
     *
     * This function extracts components of a date from a formatted string and converts them into a
     * POSIX `timespec` structure. The input string is expected to be in the following formats:
     *
     * - "MMDDhhmm"                   (basic date: month, day, hour, minute)
     * - "MMDDhhmmYYYY"               (extended with year)
     * - "MMDDhhmmYYYY.SS"            (extended with seconds)
     *
     * The returned timespec is heap-allocated and returned as a std::unique_ptr for memory safety.
     *
     * @param argument The input string representing the date.
     * @return std::unique_ptr<timespec> A pointer to a `timespec` structure representing the parsed date and time.
     *
     * @note The year must be in 4-digit format (e.g., 2025), and will be internally converted to a `tm_year` value
     *       (i.e., years since 1900). The `tv_nsec` field is always set to 0.
     *
     * @throws std::out_of_range or std::invalid_argument if the string cannot be converted to integers.
     */
    static auto ParseDate(std::string&) -> std::unique_ptr<timespec>;
};
