#include <cstdint>
#include <cstring>
#include <string>
#include <system_error>
#include <vector>

#include <sys/stat.h>
#include <unistd.h>

#include "pathResolver.hpp"

using std::generic_category;
using std::string;
using std::system_error;
using std::vector;

PathResolver::PathResolver() = default;

auto PathResolver::getLogicalPath() -> string
{
    struct stat currentStat{}; // Metadata of the current working directory
    struct stat pwdStat{};     // Metadata of the directory referenced by PWD
    char* pwd = nullptr;       // Pointer to the value of the PWD environment variable
    string pathname;           // Logical pathname of the current working directory
    string env;                // Copy of the PWD environment variable

    // Use PWD only if it identifies the current working directory
    pwd = getenv("PWD");

    // Return an empty path if PWD is not set or is empty
    if (pwd == nullptr || *pwd == '\0')
    {
        return "";
    }

    env = pwd;

    // Retrieve metadata for the directory referenced by PWD
    if (stat(env.c_str(), &pwdStat) == -1)
    {
        return "";
    }

    // Retrieve metadata for the current working directory
    if (stat(".", &currentStat) == -1)
    {
        return "";
    }

    // Compare device and inode numbers to verify that both paths identify the same directory
    if (pwdStat.st_dev == currentStat.st_dev && pwdStat.st_ino == currentStat.st_ino)
    {
        pathname = env;
    }

    // TODO: Verify that PWD is an absolute path and contains no components equal to "." or "..", as required by POSIX.
    // TODO: Verify that PWD actually refers to a directory; stat() can succeed on a regular file.

    return pathname;
}

auto PathResolver::getPhysicalPath() -> string
{
    constexpr size_t INITIAL_BUFFER_SIZE = 256;

    size_t size = INITIAL_BUFFER_SIZE; // Current buffer size in bytes
    vector<char> buffer(size);         // Buffer used to store the absolute pathname
    string pathname;                   // Absolute pathname of the current working directory

    // Retry with a larger buffer if the current one is too small
    while (true)
    {
        if (getcwd(buffer.data(), buffer.size()) != nullptr)
        {
            break;
        }

        // Prevent size_t overflow when doubling the buffer size
        if (errno == ERANGE && size <= SIZE_MAX / 2)
        {
            size = size * 2;
            buffer.resize(size);
        }
        else
        {
            throw system_error(errno, generic_category(), "getcwd");
        }
    }

    // Store the resolved physical pathname
    pathname = buffer.data();

    return pathname;
}
