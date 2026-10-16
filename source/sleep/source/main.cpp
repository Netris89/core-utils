/*
 *  Copyright (c) 2025, Ludovic Hansen
 *  License: MIT
 *
 * Description:
 * A minimal implementation of the POSIX sleep utility in C++.
 * This program waits for the specified amount of time (in seconds).
 *
 * Usage: ./sleep <time>
 *
 * The time operand must be a non-negative decimal integer, up to INT_MAX (2147483647, the largest
 * value POSIX requires). Anything else is rejected with an error message and a non-zero exit status.
 *
 *  Note: More details on options and behavior can be found here:
 *  https://pubs.opengroup.org/onlinepubs/9799919799/utilities/sleep.html
 */

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <span>
#include <stdexcept>
#include <string>
#include <thread>

using std::array;
using std::cerr;
using std::out_of_range;
using std::span;
using std::stoi;
using std::string;
using std::chrono::seconds;
using std::ranges::copy;
using std::this_thread::sleep_for;

auto main(int argc, char* argv[]) -> int
{
    span<char*> args(argv, argc); // Wrap the raw argv array in a std::span for bounds-safe access
    array<string, 2> arguments;   // Fixed-size array to store the program name and the duration argument
    seconds duration;             // Holds the duration to sleep, expressed in seconds

    // Check that exactly one argument (besides the program name) is provided
    if (args.size() != 2)
    {
        cerr << "Usage : ./sleep <duration>\n";
        return EXIT_FAILURE;
    }

    // Copies the C strings from argv into std::string objects (args holds exactly 2 elements here, so arguments is filled exactly)
    copy(args, arguments.begin());

    // POSIX requires a non-negative decimal integer: rejects an empty operand and anything that is not a digit (sign, decimal point, spaces, letters)
    if (arguments.at(1).empty() || arguments.at(1).find_first_not_of("0123456789") != string::npos)
    {
        cerr << "Invalid argument\n";
        return EXIT_FAILURE;
    }

    try
    {
        duration = seconds(stoi(arguments.at(1))); // Convert the second argument to an integer, then to chrono::seconds
    }
    catch (const out_of_range&) // The operand contains only digits, but its value does not fit in an int (above 2147483647)
    {
        cerr << "Argument out of range\n";
        return EXIT_FAILURE;
    }

    sleep_for(duration); // Pause the program for the given duration

    return EXIT_SUCCESS;
}
