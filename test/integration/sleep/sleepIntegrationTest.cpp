#include <chrono>
#include <cstdlib>

#include <gtest/gtest.h>
#include <sys/wait.h>

using std::system;
using std::chrono::seconds;
using std::chrono::steady_clock;

TEST(SleepIntegrationTest, ZeroReturnsZero)
{
    const int result = system("./sleep 0");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(SleepIntegrationTest, ZeroReturnsImmediately)
{
    const auto start = steady_clock::now();

    system("./sleep 0");

    EXPECT_LT(steady_clock::now() - start, seconds(1));
}

TEST(SleepIntegrationTest, LeadingZerosAreAccepted)
{
    const int result = system("./sleep 00");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(SleepIntegrationTest, OneSecondReturnsZero)
{
    const int result = system("./sleep 1");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(SleepIntegrationTest, OneSecondWaitsAtLeastOneSecond)
{
    const auto start = steady_clock::now();

    system("./sleep 1");

    const auto elapsed = steady_clock::now() - start;

    EXPECT_GE(elapsed, seconds(1));
    EXPECT_LT(elapsed, seconds(3));
}

TEST(SleepIntegrationTest, IntMaxIsAcceptedAndKeepsSleeping)
{
    // timeout kills the process after 1 second and returns 124: the operand was accepted and sleep was still running
    const int result = system("timeout 1 ./sleep 2147483647 > /dev/null 2>&1");

    EXPECT_EQ(WEXITSTATUS(result), 124);
}

TEST(SleepIntegrationTest, SuccessWritesNothing)
{
    const int result = system("[ -z \"$(./sleep 0 2>&1)\" ]");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(SleepIntegrationTest, NoOperandReturnsError)
{
    const int result = system("./sleep > /dev/null 2>&1");

    EXPECT_GT(WEXITSTATUS(result), 0);
}

TEST(SleepIntegrationTest, TwoOperandsReturnsError)
{
    const int result = system("./sleep 0 0 > /dev/null 2>&1");

    EXPECT_GT(WEXITSTATUS(result), 0);
}

TEST(SleepIntegrationTest, EmptyOperandReturnsError)
{
    const int result = system("./sleep \"\" > /dev/null 2>&1");

    EXPECT_GT(WEXITSTATUS(result), 0);
}

TEST(SleepIntegrationTest, LettersReturnsError)
{
    const int result = system("./sleep abc > /dev/null 2>&1");

    EXPECT_GT(WEXITSTATUS(result), 0);
}

TEST(SleepIntegrationTest, DigitsFollowedByLettersReturnsError)
{
    const int result = system("./sleep 1a > /dev/null 2>&1");

    EXPECT_GT(WEXITSTATUS(result), 0);
}

TEST(SleepIntegrationTest, NegativeNumberReturnsError)
{
    const int result = system("./sleep -1 > /dev/null 2>&1");

    EXPECT_GT(WEXITSTATUS(result), 0);
}

TEST(SleepIntegrationTest, PlusSignReturnsError)
{
    const int result = system("./sleep +1 > /dev/null 2>&1");

    EXPECT_GT(WEXITSTATUS(result), 0);
}

TEST(SleepIntegrationTest, DecimalNumberReturnsError)
{
    const int result = system("./sleep 1.5 > /dev/null 2>&1");

    EXPECT_GT(WEXITSTATUS(result), 0);
}

TEST(SleepIntegrationTest, LeadingSpaceReturnsError)
{
    const int result = system("./sleep \" 1\" > /dev/null 2>&1");

    EXPECT_GT(WEXITSTATUS(result), 0);
}

TEST(SleepIntegrationTest, TrailingSpaceReturnsError)
{
    const int result = system("./sleep \"1 \" > /dev/null 2>&1");

    EXPECT_GT(WEXITSTATUS(result), 0);
}

TEST(SleepIntegrationTest, HexadecimalReturnsError)
{
    const int result = system("./sleep 0x10 > /dev/null 2>&1");

    EXPECT_GT(WEXITSTATUS(result), 0);
}

TEST(SleepIntegrationTest, IntMaxPlusOneReturnsError)
{
    const int result = system("./sleep 2147483648 > /dev/null 2>&1");

    EXPECT_GT(WEXITSTATUS(result), 0);
}

TEST(SleepIntegrationTest, VeryLargeNumberReturnsError)
{
    const int result = system("./sleep 99999999999999999999 > /dev/null 2>&1");

    EXPECT_GT(WEXITSTATUS(result), 0);
}

TEST(SleepIntegrationTest, ErrorWritesNothingToStdout)
{
    const int result = system("[ -z \"$(./sleep abc 2>/dev/null)\" ]");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(SleepIntegrationTest, ErrorWritesDiagnosticToStderr)
{
    const int result = system("[ -n \"$(./sleep abc 2>&1 > /dev/null)\" ]");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(SleepIntegrationTest, OutOfRangeWritesDiagnosticToStderr)
{
    const int result = system("[ -n \"$(./sleep 2147483648 2>&1 > /dev/null)\" ]");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}
