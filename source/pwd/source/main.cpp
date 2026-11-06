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

#include <cstdlib>
#include <iostream>
#include <string>

#include <sys/stat.h>
#include <unistd.h>

using std::cerr;
using std::cout;

#include "pathResolver.hpp"

using std::cerr;
using std::cout;
using std::string;

auto main(int argc, char* argv[]) -> int
{
    int opt = 0;     // Stores the option returned by getopt()
    string pathname; // Absolute pathname of the current working directory

    if (argc < 2)
    {
        pathname = PathResolver::getLogicalPath();
    }

    // Parse command-line options. The last option takes precedence
    while ((opt = getopt(argc, argv, "LP")) != -1)
    {
        switch (opt)
        {
        case 'L': // Default
            pathname = PathResolver::getLogicalPath();
            if (pathname.empty())
            {
                pathname = PathResolver::getPhysicalPath();
            }
            break;
        case 'P':
            pathname = PathResolver::getPhysicalPath();
            break;
        default:
            cerr << "Invalid option. Try -L or -P." << '\n';
            return EXIT_FAILURE;
        }
    }

    if (optind < argc)
    {
        cerr << "pwd : Unexpected operand" << '\n';
        return EXIT_FAILURE;
    }

    // Write the pathname to standard output
    cout << pathname << '\n';

    // Flush stdout to detect write errors before exiting
    cout.flush();
    if (!cout)
    {
        cerr << "pwd : write error" << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
