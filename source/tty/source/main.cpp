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

#include <array>
#include <climits> // IWYU pragma: keep
#include <cstdlib>
#include <cstring>
#include <iostream>

#include <unistd.h>

using std::array;
using std::cerr;
using std::cout;
using std::strerror;

auto main() -> int
{
    constexpr int EXIT_NOT_TTY = 1;           // POSIX: 1 when standard input is not a terminal, above 1 for an error.
    constexpr int EXIT_ERROR   = 2;           // EXIT_FAILURE is 1, so it cannot be used for errors here
    array<char, TTY_NAME_MAX> terminalName{}; // Receives the terminal name written by ttyname_r(). TTY_NAME_MAX is the maximum length of a terminal device name
    int error = 0;                            // Result of ttyname_r(): 0 on success, otherwise an error number

    // Standard input is only examined, never read. isatty() returns 0 when the file descriptor does not refer to
    // a terminal, and also when it is invalid (for example a closed standard input)
    if (isatty(STDIN_FILENO) == 0)
    {
        cout << "not a tty" << '\n';
        return EXIT_NOT_TTY;
    }

    // Only called once isatty() has confirmed that standard input is a terminal, because ttyname_r() fails otherwise
    error = ttyname_r(STDIN_FILENO, terminalName.data(), terminalName.size());

    // Standard input is a terminal but its name cannot be obtained: a real error, not the "not a tty" case
    if (error != 0)
    {
        cerr << "tty: " << strerror(error) << '\n';
        return EXIT_ERROR;
    }

    cout << terminalName.data() << '\n';

    return EXIT_SUCCESS;
}
