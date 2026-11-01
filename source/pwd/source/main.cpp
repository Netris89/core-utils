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
#include <new>
#include <stdexcept>
#include <string>
#include <vector>

#include <sys/stat.h>
#include <unistd.h>

using std::bad_alloc;
using std::cerr;
using std::cout;
using std::length_error;
using std::string;
using std::vector;

auto main(int argc, char* argv[]) -> int
{
    constexpr size_t INITIAL_BUFFER_SIZE = 256;

    int opt     = 0;                   // Stores the option returned by getopt()
    size_t size = INITIAL_BUFFER_SIZE; // Current buffer size in bytes
    vector<char> buffer(size);         // Buffer used to store the absolute pathname
    string env;                        // Value of the PWD environment variable
    string pathname;                   // Absolute pathname of the current working directory
    bool logicalMode = true;           // Determines whether logical or physical path resolution is used
    struct stat currentStat{};         // Metadata of the current working directory
    struct stat pwdStat{};             // Metadata of the directory referenced by PWD

    // TODO : Reduce cognitive complexity

    // Parse command-line options. The last option takes precedence
    while ((opt = getopt(argc, argv, "LP")) != -1)
    {
        switch (opt)
        {
        case 'L': // Default
            logicalMode = true;
            break;
        case 'P':
            logicalMode = false;
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

    if (logicalMode)
    {
        // Use PWD only if it identifies the current working directory
        env = getenv("PWD");

        if (env.empty())
        {
            logicalMode = false;
        }

        // Compare device and inode numbers to verify that both paths identify the same directory
        if ((stat(env.c_str(), &pwdStat) != -1) && (stat(".", &currentStat) != -1))
        {
            // stat() follows symbolic links, which is required for logical mode
            if (pwdStat.st_dev == currentStat.st_dev && pwdStat.st_ino == currentStat.st_ino)
            {
                pathname = env;
            }
        }
    }
    else
    {
        // Retry with a larger buffer if the current one is too small
        while (getcwd(buffer.data(), buffer.size()) == nullptr)
        {
            // Prevent size_t overflow when doubling the buffer size
            if (errno == ERANGE && size <= SIZE_MAX / 2)
            {
                size = size * 2;

                try
                {
                    buffer.resize(size);
                }
                catch (const bad_alloc& error)
                {
                    cerr << "pwd : " << error.what() << '\n';
                    return EXIT_FAILURE;
                }
                catch (const length_error& error)
                {
                    cerr << "pwd : " << error.what() << '\n';
                    return EXIT_FAILURE;
                }
            }
            else
            {
                cerr << "pwd : " << strerror(errno) << '\n';
                return EXIT_FAILURE;
            }
        }

        // Store the resolved physical pathname
        pathname = buffer.data();
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
