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
#include <unistd.h>

auto main() -> int
{
    return EXIT_SUCCESS;
}
