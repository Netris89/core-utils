/*
 *  Copyright (c) 2026, Ludovic Hansen
 *  License: MIT
 *
 * Description:
 * A minimal implementation of the POSIX pwd utility in C++.
 * This program writes to the standard output an absolute pathname of the current working directory,
 * which does not contain the filenames dot or dot-dot.
 * No input is read from standard input.
 *
 *  Usage: ./pwd [-L|-P]
 *
 *  Supported options:
 *      -L      : If the PWD environment variable is an absolute pathname of the current directory without any dot or dot-dot component,
 *                write it. Otherwise behave as -P. This is the default.
 *      -P      : Write a pathname that does not contain any component referring to a symbolic link.
 *
 *  If both options are specified, the last one applies.
 *
 *  Exit status:
 *      0       : Successful completion
 *      >0      : An error occurred. Nothing is written to the standard output in that case
 *
 *  Note: More details on options and behavior can be found here:
 *  https://pubs.opengroup.org/onlinepubs/9799919799/utilities/pwd.html
 */

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>

#include <unistd.h>

using std::cerr;
using std::cout;
using std::free;
using std::unique_ptr;

auto main() -> int
{
    // TODO : getcwd(nullptr, 0) is a GNU extension. POSIX requires a caller-provided buffer.
    unique_ptr<char, decltype(&free)> cwd(getcwd(nullptr, 0), &free);

    if (cwd == nullptr)
    {
        cerr << "pwd : " << strerror(errno);
        return EXIT_FAILURE;
    }

    cout << cwd.get() << '\n';

    return EXIT_SUCCESS;
}
