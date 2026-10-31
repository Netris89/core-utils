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
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

#include <unistd.h>

using std::cerr;
using std::cout;
using std::vector;

auto main() -> int
{
    constexpr size_t INITIAL_BUFFER_SIZE = 256;

    size_t size = INITIAL_BUFFER_SIZE; // Current buffer size in bytes
    vector<char> buffer(size);         // Buffer used to store the absolute pathname

    // Retry with a larger buffer if the current one is too small
    while (getcwd(buffer.data(), buffer.size()) == nullptr)
    {
        // Prevent size_t overflow when doubling the buffer size
        if (errno == ERANGE && size <= SIZE_MAX / 2)
        {
            size = size * 2;
            buffer.resize(size);
        }
        else
        {
            cerr << "pwd : " << strerror(errno) << '\n';
            return EXIT_FAILURE;
        }
    }

    // Write the absolute pathname to standard output
    cout << buffer.data() << '\n';

    return EXIT_SUCCESS;
}
