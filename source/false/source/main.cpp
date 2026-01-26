/*
 *  Copyright (c) 2025, Ludovic Hansen
 *  License: MIT
 *
 *  Description:
 *  A minimal implementation of the POSIX false utility in C++.
 *  This program does nothing and always exits with a failure status.
 *  It can serve as a basic example of a C++ application structure or as a building block for shell scripts and system tools.
 *
 *  Note: More details on options and behavior can be found here:
 *  https://pubs.opengroup.org/onlinepubs/9799919799/utilities/false.html
 */
 
#include <cstdlib>

auto main() -> int
{
    return EXIT_FAILURE;
}
