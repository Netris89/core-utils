#pragma once

#include <string>

#include <gmock/gmock.h>

#include "clockInterface.hpp"

class MockClock : public ClockInterface
{
private:
public:
    MockClock() = default;
    MOCK_METHOD(int, getYear, (), (const, override));
    MOCK_METHOD(int, getMonth, (), (const, override));
    MOCK_METHOD(int, getDay, (), (const, override));
    MOCK_METHOD(int, getHour, (), (const, override));
    MOCK_METHOD(int, getMin, (), (const, override));
    MOCK_METHOD(int, getSec, (), (const, override));
    MOCK_METHOD(int, getDayOfTheWeek, (), (const, override));
    MOCK_METHOD(std::string, getTimeZone, (), (const, override));
};
