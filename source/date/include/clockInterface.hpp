#pragma once

#include <string>

class ClockInterface // NOLINT(cppcoreguidelines-special-member-functions)
{
private:
public:
    virtual ~ClockInterface()                       = default;
    virtual auto getYear() const -> int             = 0;
    virtual auto getMonth() const -> int            = 0;
    virtual auto getDay() const -> int              = 0;
    virtual auto getHour() const -> int             = 0;
    virtual auto getMin() const -> int              = 0;
    virtual auto getSec() const -> int              = 0;
    virtual auto getDayOfTheWeek() const -> int     = 0;
    virtual auto getTimeZone() const -> std::string = 0;
};
