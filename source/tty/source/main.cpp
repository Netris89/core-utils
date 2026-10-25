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
#include <string>

#include <unistd.h>

using std::cout;
using std::string;

auto main() -> int
{
    constexpr int EXIT_NOT_TTY = 2;
    string name;

    if (isatty(STDIN_FILENO) == 0)
    {
        cout << "not a tty" << '\n';
        return EXIT_NOT_TTY;
    }

    name = ttyname(STDIN_FILENO);

    if (name.empty())
    {
        return EXIT_FAILURE;
    }

    cout << name << '\n';

    return EXIT_SUCCESS;
}
