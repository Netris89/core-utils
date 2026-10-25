/*
 *  Copyright (c) 2026, Ludovic Hansen
 *  License: MIT
 *
 * Description:
 * A minimal implementation of the POSIX tty utility in C++.
 * This program writes to the standard output the name of the terminal that is open as standard input
 * (the name that ttyname() returns, for example /dev/pts/0).
 * If standard input is not a terminal, it writes "not a tty" instead.
 * No input is read from standard input: it is only examined to know whether it is a terminal.
 *
 *  Usage: ./tty
 *
 *  The tty utility takes neither options nor operands.
 *
 *  Exit status:
 *      0       : Standard input is a terminal, and its name was written
 *      1       : Standard input is not a terminal, and "not a tty" was written
 *      >1      : An error occurred
 *
 *  Note: More details on options and behavior can be found here:
 *  https://pubs.opengroup.org/onlinepubs/9799919799/utilities/tty.html
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
