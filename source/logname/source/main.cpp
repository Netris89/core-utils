/*
 *  Copyright (c) 2026, Ludovic Hansen
 *  License: MIT
 *
 * Description:
 * A minimal implementation of the POSIX logname utility in C++.
 * This program writes the user's login name to the standard output. The login name is the string that
 * the getlogin() function would return. The LOGNAME environment variable is deliberately ignored,
 * because environment changes could produce erroneous results.
 * Where getlogin() would fail, a diagnostic message is written to the standard error and the exit status is non-zero.
 *
 *  Usage: ./logname
 *
 *  The logname utility takes neither options nor operands.
 *
 *  Exit status:
 *      0       : Successful completion
 *      >0      : An error occurred
 *
 *  Note: More details on options and behavior can be found here:
 *  https://pubs.opengroup.org/onlinepubs/9799919799/utilities/logname.html
 */

#include <cstdlib>
#include <iostream>
#include <string>

#include <unistd.h>

using std::cout;
using std::string;

auto main() -> int
{
    string user = getlogin();

    if (user.empty())
    {
        return EXIT_FAILURE;
    }

    cout << user << '\n';

    return EXIT_SUCCESS;
}
