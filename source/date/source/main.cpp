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

#include <cctype>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

#include <getopt.h>

#include "clock.hpp"
#include "parser.hpp"

using std::cerr;
using std::cout;
using std::string;
using std::vector;

auto main(int argc, char* argv[]) -> int
{
    vector<string> arguments(argv + 1, argv + argc); // Vector of arguments from command-line inputs
    bool isUtc = false;                              // Indicates that the time should be printed in UTC instead of local time.
    int opt    = 0;                                  // Result of getopt
    Clock clock;                                     // Object for the clock
    string formattedDate;                            // Formatted date

    // Check if getop returns -1. If it does, handle the option
    while ((opt = getopt(argc, argv, "u")) != -1)
    {
        switch (opt)
        {
        case 'u':
            isUtc = true;
            break;
        default:
            cerr << "Invalid option. Try -u if you want to set time in UTC.";
            return EXIT_FAILURE;
        }
    }

    // if there are more than 3 arguments, prints the usage and terminates the program
    if (arguments.size() > 3)
    {
        cerr << "Usage : ./date [-u] [+format]";
        return EXIT_FAILURE;
    }

    clock = Clock(isUtc);

    // Parse the arguments. If one starts with +, it's the formatting argument. If one start with a digit, it's to set the system date
    for (string argument : arguments)
    {
        if (argument == "-u")
        {
            continue;
        }
        if (isdigit(argument.front()) != 0)
        {
            Clock::setTime(Parser::ParseDate(argument).get());
            return EXIT_SUCCESS;
        }

        formattedDate = Parser::ParseFormat(argument, clock);
    }

    // If formattedDate is empty, no formatting argument has been passed so the whole date shall be printed
    if (formattedDate.empty())
    {
        string arg    = "+%a %b %e %H:%M:%S %Z %Y";
        formattedDate = Parser::ParseFormat(arg, clock);
    }

    cout << formattedDate;

    return EXIT_SUCCESS;
}