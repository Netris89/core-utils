/*
 *  Copyright (c) 2026, Ludovic Hansen
 *  License: MIT
 *
 * Description:
 * A minimal implementation of the POSIX uname utility in C++.
 * This program writes information about the system to the standard output, using the symbols
 * that the uname() function reports. By default, only the operating system name is written.
 * When options are given, only the selected symbols are written, always in the same order
 * (system name, node name, release, version, machine) and separated by a single space.
 *
 *  Usage: ./uname [-amnrsv]
 *
 *  Supported options:
 *      -a      : Behave as though all of the options -mnrsv were specified
 *      -m      : Write the name of the hardware type on which the system is running
 *      -n      : Write the name of this node within an implementation-defined communications network
 *      -r      : Write the current release level of the operating system implementation
 *      -s      : Write the name of the implementation of the operating system. This is the default.
 *      -v      : Write the current version level of this release of the operating system implementation
 *
 *  The uname utility takes no operands.
 *
 *  Exit status:
 *      0       : The requested information was successfully written
 *      >0      : An error occurred
 *
 *  Note: More details on options and behavior can be found here:
 *  https://pubs.opengroup.org/onlinepubs/9799919799/utilities/uname.html
 */

#include <cstdlib>
#include <iostream>
#include <ranges>

#include <span>
#include <string>
#include <sys/utsname.h>
#include <unistd.h>
#include <vector>

using std::cout;
using std::span;
using std::string;
using std::vector;
using std::ranges::data;

auto main(int argc, char* argv[]) -> int
{
    span<char*> args(argv, argc);                           // Wraps the raw argv array in a std::span so it can be sliced without pointer arithmetic
    vector<string> arguments(args.begin() + 1, args.end()); // Command-line arguments, without the program name (args[0])
    const char* sysname = nullptr;                          // Pointer to the first character of the system name
    utsname info{};                                         // Structure that will hold the system information returned by uname()

    if (uname(&info) == -1)
    {
        return EXIT_FAILURE;
    }

    // Obtain a pointer to the first element of the sysname character array.
    // This avoids an implicit array-to-pointer conversion at the call site
    sysname = data(info.sysname);

    cout << sysname << '\n';

    return EXIT_SUCCESS;
}
