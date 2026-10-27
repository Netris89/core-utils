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

#include <array>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <ranges>
#include <string>

#include <sys/utsname.h>
#include <unistd.h>

using std::array;
using std::cerr;
using std::cout;
using std::string;
using std::ranges::data;

auto main(int argc, char* argv[]) -> int
{
    string sysinfo;                           // Stores the system name and selected system information fields
    utsname info{};                           // Stores the system information returned by uname()
    int opt                         = 0;      // Stores the option returned by getopt()
    bool sysname                    = false;  // Determines if system name is added to the printed sysinfo
    bool nodename                   = false;  // Determines if the node name is added to the printed sysinfo
    bool release                    = false;  // Determines if the release is added to the printed sysinfo
    bool version                    = false;  // Determines if the version is added to the printed sysinfo
    bool machine                    = false;  // Determines if the machine is added to the printed sysinfo
    constexpr size_t OPTION_COUNT   = 5;      // Number of supported uname options
    constexpr size_t SYS_INFO_COUNT = 5;      // Number of system information fields returned by uname()
    array<bool, OPTION_COUNT> options{};      // Array containing the different options to print
    array<string, SYS_INFO_COUNT> sysinfos{}; // Array containing the different system info

    if (uname(&info) == -1)
    {
        cerr << "uname: failed to retrieve system information" << '\n';
        return EXIT_FAILURE;
    }

    sysinfos = {
        data(info.sysname),
        data(info.nodename),
        data(info.release),
        data(info.version),
        data(info.machine),
    };

    if (argc == 1)
    {
        sysname = true;
    }

    while ((opt = getopt(argc, argv, "amnrsv")) != -1)
    {
        switch (opt)
        {
        case 'a':
            sysname  = true;
            nodename = true;
            release  = true;
            version  = true;
            machine  = true;
            break;
        case 'm':
            machine = true;
            break;
        case 'n':
            nodename = true;
            break;
        case 'r':
            release = true;
            break;
        case 's':
            sysname = true;
            break;
        case 'v':
            version = true;
            break;
        case '?':
            cerr << "uname: invalid option -- '" << static_cast<char>(optopt) << "'\n";
            return EXIT_FAILURE;
        default:
            sysname = true;
            break;
        }
    }

    options = {
        sysname,
        nodename,
        release,
        version,
        machine,
    };

    for (size_t i = 0; i < options.size(); i++)
    {
        if (options.at(i))
        {
            sysinfo += sysinfos.at(i);

            if (i < options.size() - 1)
            {
                sysinfo += ' ';
            }
        }
    }

    // The uname utility does not accept operands
    if (optind < argc)
    {
        cerr << "uname: operands are not supported" << '\n';
        return EXIT_FAILURE;
    }

    cout << sysinfo << '\n';

    return EXIT_SUCCESS;
}
