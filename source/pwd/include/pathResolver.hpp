#pragma once

#include <string>

class PathResolver
{
private:

public:
    PathResolver();
    auto static getLogicalPath() -> std::string;
    auto static getPhysicalPath() -> std::string;
};
