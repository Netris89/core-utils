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

TEST(ParserDateTests, RejectsArgumentShorterThanBaseLength)
{
    string tooShort = "100314"; // 6 chars, below the minimum length

    EXPECT_EQ(Parser::ParseDate(tooShort), nullptr);
}

TEST(ParserDateTests, DefaultsToCurrentYearWhenYearNotProvided)
{
    string minimal = "10031430";

    auto result = Parser::ParseDate(minimal);

    ASSERT_NE(result, nullptr);

    tm checked        = {};
    time_t parsedTime = result->tv_sec;
    localtime_r(&parsedTime, &checked);

    time_t now = time(nullptr);
    tm nowTm   = {};
    localtime_r(&now, &nowTm);

    // KNOWN ISSUE: currently fails, year defaults to 0 (tm_year=-1900)
    // instead of the current year.
    EXPECT_EQ(checked.tm_year, nowTm.tm_year);
}

TEST(ParserDateTests, ParsesFullFormatWithYearAndSeconds)
{
    string full = "100314302026.30"; // month day hour minute year . seconds

    auto result = Parser::ParseDate(full);

    ASSERT_NE(result, nullptr);

    tm checked        = {};
    time_t parsedTime = result->tv_sec;
    localtime_r(&parsedTime, &checked);

    EXPECT_EQ(checked.tm_mon, 9);
    EXPECT_EQ(checked.tm_mday, 3);
    EXPECT_EQ(checked.tm_hour, 14);
    EXPECT_EQ(checked.tm_min, 30);
    EXPECT_EQ(checked.tm_year, 126); // 2026 - 1900
    EXPECT_EQ(checked.tm_sec, 30);
}

TEST(ParserDateTests, DoesNotCrashWithYearButNoSeconds)
{
    string noSeconds = "100314302026"; // 12 chars: year present, no seconds

    EXPECT_NO_THROW({
        auto result = Parser::ParseDate(noSeconds);
        ASSERT_NE(result, nullptr);
    });
}

TEST(ParserDateTests, RejectsOutOfRangeMonth)
{
    string invalidMonth = "13031430";

    EXPECT_EQ(Parser::ParseDate(invalidMonth), nullptr);
}

TEST(ParserDateTests, RejectsNonNumericMonth)
{
    string nonNumeric = "ab031430";

    EXPECT_EQ(Parser::ParseDate(nonNumeric), nullptr);
}

TEST(ParserFormatTests, Hour12Format) // %I
{
    MockClock clock;
    string format = "+%I";

    ON_CALL(clock, getHour()).WillByDefault(Return(14)); // NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)

    EXPECT_EQ(Parser::ParseFormat(format, clock), "02\n");
}

TEST(ParserFormatTests, AmIndicator) // %p
{
    MockClock clock;
    string format = "+%p";

    ON_CALL(clock, getHour()).WillByDefault(Return(9)); // NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)

    EXPECT_EQ(Parser::ParseFormat(format, clock), "AM\n");
}

TEST(ParserFormatTests, PmIndicator) // %p
{
    MockClock clock;
    string format = "+%p";

    ON_CALL(clock, getHour()).WillByDefault(Return(14)); // NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)

    EXPECT_EQ(Parser::ParseFormat(format, clock), "PM\n");
}

TEST(ParserFormatTests, IsoWeekday) // %u: 1=Monday .. 7=Sunday
{
    MockClock clock;
    string format = "+%u";

    ON_CALL(clock, getDayOfTheWeek()).WillByDefault(Return(0)); // Sunday (tm_wday convention)

    EXPECT_EQ(Parser::ParseFormat(format, clock), "7\n");
}

TEST(ParserFormatTests, NumericWeekday) // %w: 0=Sunday .. 6=Saturday
{
    MockClock clock;
    string format = "+%w";

    ON_CALL(clock, getDayOfTheWeek()).WillByDefault(Return(4)); // Thursday

    EXPECT_EQ(Parser::ParseFormat(format, clock), "4\n");
}

TEST(ParserFormatTests, Full12HourTime) // %r, POSIX: equivalent to %I:%M:%S %p
{
    MockClock clock;
    string format = "+%r";

    ON_CALL(clock, getHour()).WillByDefault(Return(13)); // NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
    ON_CALL(clock, getMin()).WillByDefault(Return(36)); // NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
    ON_CALL(clock, getSec()).WillByDefault(Return(32)); // NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)

    // same HH:MM output as %R instead of %I:%M:%S %p.
    EXPECT_EQ(Parser::ParseFormat(format, clock), "01:36:32 PM\n");
}

TEST(ParserDateTests, InfersTwentyFirstCenturyForLowTwoDigitYear)
{
    string argument = "1003143026"; // mm dd hh MM + yy=26, no century

    auto result = Parser::ParseDate(argument);

    ASSERT_NE(result, nullptr);

    tm checked        = {};
    time_t parsedTime = result->tv_sec;
    localtime_r(&parsedTime, &checked);

    EXPECT_EQ(checked.tm_year, 126); // 2026 - 1900
}

TEST(ParserDateTests, InfersTwentiethCenturyForHighTwoDigitYear)
{
    string argument = "1003143085"; // mm dd hh MM + yy=85, no century

    auto result = Parser::ParseDate(argument);

    ASSERT_NE(result, nullptr);

    tm checked        = {};
    time_t parsedTime = result->tv_sec;
    localtime_r(&parsedTime, &checked);

    EXPECT_EQ(checked.tm_year, 85); // 1985 - 1900
}