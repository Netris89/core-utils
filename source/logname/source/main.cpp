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

#include <array>
#include <climits> // IWYU pragma: keep
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

#include <unistd.h>

using std::array;
using std::cerr;
using std::cout;
using std::strerror;
using std::string;

auto main() -> int
{
    array<char, LOGIN_NAME_MAX> buffer{}; // Receives the login name. getlogin_r writes into it, so no pointer owned by the C library is involved
    string user;                          // The login name as a std::string, built only once getlogin_r has succeeded

    // Unlike getlogin(), it does not return a pointer that may be null: it returns 0 on success
    // or an error number (for example when the process has no controlling terminal)
    int error = getlogin_r(buffer.data(), buffer.size());

    // POSIX requires a diagnostic message on the standard error and a non-zero exit status.
    // Nothing is written to the standard output in that case
    if (error != 0)
    {
        cerr << "logname : " << strerror(error) << '\n';
        return EXIT_FAILURE;
    }

    // On success the buffer holds a null-terminated string, so it can be converted safely
    user = buffer.data();

    cout << user << '\n';

    return EXIT_SUCCESS;
}
