#include <cstdlib>

#include <gtest/gtest.h>
#include <sys/wait.h>

using std::system;

// getlogin_r() depends on the environment (controlling terminal, session), so the system logname is used as reference:
// the success tests are skipped where it fails, and the failure tests are skipped where it works
static auto systemLognameWorks() -> bool
{
    return WEXITSTATUS(system("logname > /dev/null 2>&1")) == 0;
}

TEST(LognameIntegrationTest, SucceedsWhenSystemLognameSucceeds)
{
    const int result = system("./logname > /dev/null 2>&1; a=$?; logname > /dev/null 2>&1; b=$?; [ $((a > 0)) -eq $((b > 0)) ]");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(LognameIntegrationTest, ReturnsZero)
{
    if (!systemLognameWorks())
    {
        GTEST_SKIP() << "The system logname fails in this environment";
    }

    const int result = system("./logname > /dev/null");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(LognameIntegrationTest, WritesSameNameAsSystemLogname)
{
    if (!systemLognameWorks())
    {
        GTEST_SKIP() << "The system logname fails in this environment";
    }

    const int result = system("./logname > logname_output.txt && logname | cmp -s - logname_output.txt");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(LognameIntegrationTest, IgnoresLognameEnvironmentVariable)
{
    if (!systemLognameWorks())
    {
        GTEST_SKIP() << "The system logname fails in this environment";
    }

    const int result = system("LOGNAME=not_a_real_user ./logname > logname_output.txt && logname | cmp -s - logname_output.txt");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(LognameIntegrationTest, OutputEndsWithSingleNewline)
{
    if (!systemLognameWorks())
    {
        GTEST_SKIP() << "The system logname fails in this environment";
    }

    const int result = system("./logname > logname_output.txt && [ $(wc -l < logname_output.txt) -eq 1 ]");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(LognameIntegrationTest, WritesNothingToStderrOnSuccess)
{
    if (!systemLognameWorks())
    {
        GTEST_SKIP() << "The system logname fails in this environment";
    }

    const int result = system("[ -z \"$(./logname 2>&1 > /dev/null)\" ]");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(LognameIntegrationTest, FailureReturnsError)
{
    if (systemLognameWorks())
    {
        GTEST_SKIP() << "The system logname works in this environment";
    }

    const int result = system("./logname > /dev/null 2>&1");

    EXPECT_GT(WEXITSTATUS(result), 0);
}

TEST(LognameIntegrationTest, FailureWritesNothingToStdout)
{
    if (systemLognameWorks())
    {
        GTEST_SKIP() << "The system logname works in this environment";
    }

    const int result = system("[ -z \"$(./logname 2>/dev/null)\" ]");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(LognameIntegrationTest, FailureWritesDiagnosticToStderr)
{
    if (systemLognameWorks())
    {
        GTEST_SKIP() << "The system logname works in this environment";
    }

    const int result = system("[ -n \"$(./logname 2>&1 > /dev/null)\" ]");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}
