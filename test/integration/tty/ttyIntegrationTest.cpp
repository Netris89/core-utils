#include <cstdlib>

#include <gtest/gtest.h>
#include <sys/wait.h>
#include <unistd.h>

using std::system;

TEST(TtyIntegrationTest, DevNullReturnsOne)
{
    const int result = system("./tty < /dev/null > /dev/null");

    EXPECT_EQ(WEXITSTATUS(result), 1);
}

TEST(TtyIntegrationTest, DevNullWritesNotATty)
{
    const int result = system("./tty < /dev/null > tty_output.txt; printf 'not a tty\\n' | cmp -s - tty_output.txt");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(TtyIntegrationTest, PipeReturnsOne)
{
    const int result = system("echo | ./tty > /dev/null");

    EXPECT_EQ(WEXITSTATUS(result), 1);
}

TEST(TtyIntegrationTest, PipeWritesNotATty)
{
    const int result = system("echo | ./tty > tty_output.txt; printf 'not a tty\\n' | cmp -s - tty_output.txt");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(TtyIntegrationTest, RegularFileReturnsOne)
{
    const int result = system("./tty < ./tty > /dev/null");

    EXPECT_EQ(WEXITSTATUS(result), 1);
}

TEST(TtyIntegrationTest, ClosedStdinReturnsOne)
{
    const int result = system("./tty <&- > /dev/null 2>&1");

    EXPECT_EQ(WEXITSTATUS(result), 1);
}

TEST(TtyIntegrationTest, ClosedStdinWritesNotATty)
{
    const int result = system("./tty <&- > tty_output.txt 2>/dev/null; printf 'not a tty\\n' | cmp -s - tty_output.txt");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(TtyIntegrationTest, DoesNotConsumeStandardInput)
{
    const int result = system("printf 'data\\n' | (./tty; cat) > tty_output.txt; printf 'not a tty\\ndata\\n' | cmp -s - tty_output.txt");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(TtyIntegrationTest, NotATtyWritesNothingToStderr)
{
    const int result = system("[ -z \"$(./tty < /dev/null 2>&1 > /dev/null)\" ]");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(TtyIntegrationTest, MatchesSystemTtyWhenNotATty)
{
    const int result = system("./tty < /dev/null > /dev/null; a=$?; tty < /dev/null > /dev/null; b=$?; [ $a -eq $b ]");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(TtyIntegrationTest, TerminalReturnsZero)
{
    if (isatty(STDIN_FILENO) == 0)
    {
        GTEST_SKIP() << "Standard input of the test is not a terminal";
    }

    const int result = system("./tty > /dev/null");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(TtyIntegrationTest, TerminalWritesSameNameAsSystemTty)
{
    if (isatty(STDIN_FILENO) == 0)
    {
        GTEST_SKIP() << "Standard input of the test is not a terminal";
    }

    const int result = system("./tty > tty_output.txt && tty | cmp -s - tty_output.txt");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(TtyIntegrationTest, TerminalNameIsADevicePath)
{
    if (isatty(STDIN_FILENO) == 0)
    {
        GTEST_SKIP() << "Standard input of the test is not a terminal";
    }

    const int result = system("./tty | grep -q '^/dev/'");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}
