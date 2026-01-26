#include <string>

#include "gmock/gmock.h"
#include <gtest/gtest.h>

#include "mockClock.hpp"
#include "parser.hpp"

using std::string;
using testing::Return;

TEST(ParserFormatTests, DoublePercent)
{
    MockClock clock;
    string format = "+%%a";

    EXPECT_EQ(Parser::ParseFormat(format, clock), "%a\n");
}

TEST(ParserFormatTests, FormatsShortNumericDate)
{
    MockClock clock;
    string format = "+%d/%m/%y";

    ON_CALL(clock, getDay()).WillByDefault(Return(2));   // NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
    ON_CALL(clock, getMonth()).WillByDefault(Return(2)); // NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
    ON_CALL(clock, getYear()).WillByDefault(Return(99)); // NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)

    EXPECT_EQ(Parser::ParseFormat(format, clock), "02/02/99\n");
}

TEST(ParserFormatTests, LongTimePlaceholder)
{
    MockClock clock;
    string format = "+%T";

    ON_CALL(clock, getHour()).WillByDefault(Return(14)); // NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
    ON_CALL(clock, getMin()).WillByDefault(Return(15));  // NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
    ON_CALL(clock, getSec()).WillByDefault(Return(16));  // NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)

    EXPECT_EQ(Parser::ParseFormat(format, clock), "14:15:16\n");
}

TEST(ParserFormatTests, ShortTimePlaceholder)
{
    MockClock clock;
    string format = "+%R";

    ON_CALL(clock, getHour()).WillByDefault(Return(14)); // NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
    ON_CALL(clock, getMin()).WillByDefault(Return(15));  // NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)

    EXPECT_EQ(Parser::ParseFormat(format, clock), "14:15\n");
}

TEST(ParserFormatTests, FullFormat)
{
    MockClock clock;
    string format = "+%a %b %e %H:%M:%S %Z %Y";

    ON_CALL(clock, getDayOfTheWeek()).WillByDefault(Return(4));
    ON_CALL(clock, getMonth()).WillByDefault(Return(11)); // NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
    ON_CALL(clock, getDay()).WillByDefault(Return(26));   // NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
    ON_CALL(clock, getHour()).WillByDefault(Return(19));  // NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
    ON_CALL(clock, getMin()).WillByDefault(Return(45));   // NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
    ON_CALL(clock, getSec()).WillByDefault(Return(00));
    ON_CALL(clock, getTimeZone()).WillByDefault(Return("MSK"));
    ON_CALL(clock, getYear()).WillByDefault(Return(1991)); // NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)

    EXPECT_EQ(Parser::ParseFormat(format, clock), "Thu Dec 26 19:45:00 MSK 1991\n");
}

TEST(ParserFormatTests, FullDay)
{
    MockClock clock;
    string format = "+%A";

    ON_CALL(clock, getDayOfTheWeek()).WillByDefault(Return(1)); // NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)

    EXPECT_EQ(Parser::ParseFormat(format, clock), "Monday\n");
}

TEST(ParserFormatTests, FullMonth)
{
    MockClock clock;
    string format = "+%B";

    ON_CALL(clock, getMonth()).WillByDefault(Return(7)); // NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)

    EXPECT_EQ(Parser::ParseFormat(format, clock), "August\n");
}

TEST(ParserFormatTests, MixedTextAndFormat)
{
    MockClock clock;
    string format = "+Now: %Hh %Mmin %Ssec";

    ON_CALL(clock, getHour()).WillByDefault(Return(8)); // NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
    ON_CALL(clock, getMin()).WillByDefault(Return(5));  // NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
    ON_CALL(clock, getSec()).WillByDefault(Return(30)); // NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)

    EXPECT_EQ(Parser::ParseFormat(format, clock), "Now: 08h 05min 30sec\n");
}

TEST(ParserFormatTests, ReturnsZeroPaddedHour)
{
    MockClock clock;
    string format = "+%H";

    ON_CALL(clock, getHour()).WillByDefault(Return(3));

    EXPECT_EQ(Parser::ParseFormat(format, clock), "03\n");
}

TEST(ParserFormatTests, StaticTextOnly)
{
    MockClock clock;
    string format = "static text only";

    EXPECT_EQ(Parser::ParseFormat(format, clock), "static text only\n");
}

TEST(ParserFormatTests, TrailingPercent)
{
    MockClock clock;
    string format = "Ends with percent%";

    EXPECT_EQ(Parser::ParseFormat(format, clock), "Ends with percent%\n");
}

TEST(ParserFormatTests, UnknownPlaceholder)
{
    MockClock clock;
    string format = "Date: %x";

    EXPECT_EQ(Parser::ParseFormat(format, clock), "Date: %x\n");
}