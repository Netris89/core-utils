/*
 *  Copyright (c) 2025, Ludovic Hansen
 *  License: MIT
 *
 * Description:
 * A minimal implementation of the POSIX sleep utility in C++.
 * This program waits for the specified amound of time (in second).
 *
 *  Note: More details on options and behavior can be found here:
 *  https://pubs.opengroup.org/onlinepubs/9799919799/utilities/sleep.html
 */

#include <array>
#include <chrono>
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <span>
#include <string>
#include <thread>

using std::array;
using std::cerr;
using std::invalid_argument;
using std::span;
using std::stoi;
using std::string;
using std::chrono::seconds;
using std::this_thread::sleep_for;

auto main(int argc, char* argv[]) -> int
{
    span<char*> args(argv, argc); // Wrap the raw argv array in a std::span for bounds-safe access
    array<string, 2> arguments;   // Fixed-size array to store the program name and the duration argument
    seconds duration;             // hold the duration to sleep, expressed in seconds

    // Check that exactly one argument (besides the program name) is provided
    if (args.size() != 2)
    {
        cerr << "Usage : ./sleep <duration>\n";
        return EXIT_FAILURE;
    }

    arguments = {args[0], args[1]}; // Store the arguments as std::string for easier manipulation

    try
    {
        duration = seconds(stoi(arguments.at(1))); // Convert the second argument to an integer, then to chrono::seconds
    }
    catch (const invalid_argument& e) // The argument is not a valid number
    {
        cerr << "Argument is not a numerical value\n";
        return EXIT_FAILURE;
    }

    if (duration.count() < 0) // If the duration is a negative number, exits with a failure
    {
        cerr << "Can't sleep for a negative amount of time\n";
        return EXIT_FAILURE;
    }

    sleep_for(duration); // Pause the program for the given duration

    return EXIT_SUCCESS;
}
