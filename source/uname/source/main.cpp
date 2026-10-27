/*
 *  Copyright (c) 2026, Ludovic Hansen
 *  License: MIT
 *
 * Description:
 * A minimal implementation of the POSIX uname utility in C++.
 * This program writes information about the system to the standard output, using the symbols
 * that the uname() function reports. By default, only the operating system name is written.
 * When options are given, only the selected symbols are written, always in the same order
 * (system name, node name, release, version, machine) and separated by a single space.
 *
 *  Usage: ./uname [-amnrsv]
 *
 *  Supported options:
 *      -a      : Behave as though all of the options -mnrsv were specified
 *      -m      : Write the name of the hardware type on which the system is running
 *      -n      : Write the name of this node within an implementation-defined communications network
 *      -r      : Write the current release level of the operating system implementation
 *      -s      : Write the name of the implementation of the operating system. This is the default.
 *      -v      : Write the current version level of this release of the operating system implementation
 *
 *  The uname utility takes no operands.
 *
 *  Exit status:
 *      0       : The requested information was successfully written
 *      >0      : An error occurred
 *
 *  Note: More details on options and behavior can be found here:
 *  https://pubs.opengroup.org/onlinepubs/9799919799/utilities/uname.html
 */

#include <cstdlib>
#include <iostream>
#include <ranges>
#include <string>

#include <getopt.h>
#include <sys/utsname.h>
#include <unistd.h>

using std::cerr;
using std::cout;
using std::string;
using std::ranges::data;

auto main(int argc, char* argv[]) -> int
{
    string sysname;                                         // Stores the system name and selected system information fields.
    utsname info{};                                         // Stores the system information returned by uname().
    int opt = 0;                                            // Stores the option returned by getopt().

    if (uname(&info) == -1)
    {
        return EXIT_FAILURE;
    }

    while ((opt = getopt(argc, argv, "amnrsv")) != -1)
    {
        switch (opt)
        {
        case 'a':
            sysname = data(info.sysname);
            sysname += ' ';
            sysname += data(info.nodename);
            sysname += ' ';
            sysname += data(info.release);
            sysname += ' ';
            sysname += data(info.version);
            sysname += ' ';
            sysname += data(info.machine);
            break;
        case 'm':
            sysname += data(info.machine);
            break;
        case 'n':
            sysname += data(info.nodename);
            break;
        case 'r':
            sysname += data(info.release);
            break;
        case 's':
            sysname += data(info.sysname);
            break;
        case 'v':
            sysname += data(info.version);
            break;
        case '?':
            cerr << "uname: invalid option -- '" << static_cast<char>(optopt) << "'\n";
            return EXIT_FAILURE;
        default:
            sysname = data(info.sysname);
            break;
        }
    }

    // The uname utility does not accept operands
    if (optind < argc)
    {
        cerr << "uname: operands are not supported" << '\n';
        return EXIT_FAILURE;
    }

    if (sysname.empty())
    {
        sysname = data(info.sysname);
    }

    cout << sysname << '\n';

    return EXIT_SUCCESS;
}
