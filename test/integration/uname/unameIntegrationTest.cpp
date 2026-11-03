#include <cstdlib>

#include <gtest/gtest.h>
#include <sys/wait.h>

using std::system;

TEST(UnameIntegrationTest, ReturnsZero)
{
    const int result = system("./uname > /dev/null");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(UnameIntegrationTest, NoOptionWritesSysname)
{
    const int result = system("./uname > uname_output.txt && uname -s | cmp -s - uname_output.txt");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(UnameIntegrationTest, DoubleDashWritesSysname)
{
    const int result = system("./uname -- > uname_output.txt && uname -s | cmp -s - uname_output.txt");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(UnameIntegrationTest, OptionSWritesSysname)
{
    const int result = system("./uname -s > uname_output.txt && uname -s | cmp -s - uname_output.txt");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(UnameIntegrationTest, OptionNWritesNodename)
{
    const int result = system("./uname -n > uname_output.txt && uname -n | cmp -s - uname_output.txt");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(UnameIntegrationTest, OptionRWritesRelease)
{
    const int result = system("./uname -r > uname_output.txt && uname -r | cmp -s - uname_output.txt");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(UnameIntegrationTest, OptionVWritesVersion)
{
    const int result = system("./uname -v > uname_output.txt && uname -v | cmp -s - uname_output.txt");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(UnameIntegrationTest, OptionMWritesMachine)
{
    const int result = system("./uname -m > uname_output.txt && uname -m | cmp -s - uname_output.txt");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(UnameIntegrationTest, OptionAWritesAllSymbolsInFixedOrder)
{
    const int result = system("./uname -a > uname_output.txt && "
                              "printf '%s %s %s %s %s\\n' \"$(uname -s)\" \"$(uname -n)\" \"$(uname -r)\" \"$(uname -v)\" \"$(uname -m)\" "
                              "| cmp -s - uname_output.txt");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(UnameIntegrationTest, OptionAIsEquivalentToMnrsv)
{
    const int result = system("[ \"$(./uname -a)\" = \"$(./uname -mnrsv)\" ]");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(UnameIntegrationTest, OptionAWithOtherOptionsStillWritesAllSymbols)
{
    const int result = system("[ \"$(./uname -a)\" = \"$(./uname -a -s)\" ]");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(UnameIntegrationTest, OutputOrderIsFixedWhateverTheOptionOrder)
{
    const int result = system("./uname -mv > uname_output.txt && "
                              "printf '%s %s\\n' \"$(uname -v)\" \"$(uname -m)\" | cmp -s - uname_output.txt");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(UnameIntegrationTest, ReversedOptionsGiveSameOutput)
{
    const int result = system("[ \"$(./uname -mnrsv)\" = \"$(./uname -vsrnm)\" ]");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(UnameIntegrationTest, SeparateOptionsAreEquivalentToGroupedOptions)
{
    const int result = system("[ \"$(./uname -s -n -m)\" = \"$(./uname -snm)\" ]");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(UnameIntegrationTest, RepeatedOptionsAreWrittenOnce)
{
    const int result = system("[ \"$(./uname -ss)\" = \"$(./uname -s)\" ]");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(UnameIntegrationTest, OutputEndsWithSingleNewline)
{
    const int result = system("[ \"$(./uname -a | wc -l)\" -eq 1 ]");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(UnameIntegrationTest, OperandReturnsError)
{
    const int result = system("./uname foo > /dev/null 2>&1");

    EXPECT_GT(WEXITSTATUS(result), 0);
}

TEST(UnameIntegrationTest, OperandAfterOptionReturnsError)
{
    const int result = system("./uname -s foo > /dev/null 2>&1");

    EXPECT_GT(WEXITSTATUS(result), 0);
}

TEST(UnameIntegrationTest, OperandAfterDoubleDashReturnsError)
{
    const int result = system("./uname -- foo > /dev/null 2>&1");

    EXPECT_GT(WEXITSTATUS(result), 0);
}

TEST(UnameIntegrationTest, UnknownOptionReturnsError)
{
    const int result = system("./uname -z > /dev/null 2>&1");

    EXPECT_GT(WEXITSTATUS(result), 0);
}

TEST(UnameIntegrationTest, UnknownOptionAmongValidOnesReturnsError)
{
    const int result = system("./uname -sz > /dev/null 2>&1");

    EXPECT_GT(WEXITSTATUS(result), 0);
}
