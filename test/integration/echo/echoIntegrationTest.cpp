#include <cstdlib>

#include <gtest/gtest.h>
#include <sys/wait.h>

using std::system;

// The "x" sentinel written after the output keeps the final newline, which $(...) would otherwise strip

TEST(EchoIntegrationTest, ReturnsZero)
{
    const int result = system("./echo hello > /dev/null");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(EchoIntegrationTest, NoArgumentWritesNewline)
{
    const int result = system(R"sh([ "$(./echo; printf x)" = "$(printf '\nx')" ])sh");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(EchoIntegrationTest, OneArgumentIsFollowedByNewline)
{
    const int result = system(R"sh([ "$(./echo hello; printf x)" = "$(printf '%s\nx' 'hello')" ])sh");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(EchoIntegrationTest, ArgumentsAreSeparatedBySingleSpace)
{
    const int result = system(R"sh([ "$(./echo a b c; printf x)" = "$(printf '%s\nx' 'a b c')" ])sh");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(EchoIntegrationTest, ManyArgumentsAreAllWritten)
{
    const int result = system(R"sh([ "$(./echo 1 2 3 4 5 6 7 8 9 10; printf x)" = "$(printf '%s\nx' '1 2 3 4 5 6 7 8 9 10')" ])sh");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(EchoIntegrationTest, EmptyArgumentWritesNewline)
{
    const int result = system(R"sh([ "$(./echo ""; printf x)" = "$(printf '\nx')" ])sh");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(EchoIntegrationTest, EmptyArgumentsAreStillSeparatedBySpaces)
{
    const int result = system(R"sh([ "$(./echo "" ""; printf x)" = "$(printf '%s\nx' ' ')" ])sh");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(EchoIntegrationTest, InnerSpacesArePreserved)
{
    const int result = system(R"sh([ "$(./echo "a  b"; printf x)" = "$(printf '%s\nx' 'a  b')" ])sh");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(EchoIntegrationTest, LeadingAndTrailingSpacesArePreserved)
{
    const int result = system(R"sh([ "$(./echo " a "; printf x)" = "$(printf '%s\nx' ' a ')" ])sh");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(EchoIntegrationTest, SpecialCharactersAreNotInterpreted)
{
    const int result = system(R"sh([ "$(./echo '*' '$HOME' '~'; printf x)" = "$(printf '%s\nx' '* $HOME ~')" ])sh");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(EchoIntegrationTest, DoubleDashIsWrittenAsAString)
{
    const int result = system(R"sh([ "$(./echo --; printf x)" = "$(printf '%s\nx' '--')" ])sh");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(EchoIntegrationTest, DoubleDashAmongOperandsIsWrittenAsAString)
{
    const int result = system(R"sh([ "$(./echo -- a; printf x)" = "$(printf '%s\nx' '-- a')" ])sh");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(EchoIntegrationTest, SingleDashIsWrittenAsAString)
{
    const int result = system(R"sh([ "$(./echo -; printf x)" = "$(printf '%s\nx' '-')" ])sh");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(EchoIntegrationTest, UnknownOptionIsWrittenAsAString)
{
    const int result = system(R"sh([ "$(./echo -z; printf x)" = "$(printf '%s\nx' '-z')" ] && )sh"
                              R"sh([ "$(./echo -x foo; printf x)" = "$(printf '%s\nx' '-x foo')" ])sh");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(EchoIntegrationTest, LongOptionLikeOperandsAreWrittenAsStrings)
{
    const int result = system(R"sh([ "$(./echo --help --version; printf x)" = "$(printf '%s\nx' '--help --version')" ])sh");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(EchoIntegrationTest, FirstOperandMadeOfEAndNIsWrittenAsAString)
{
    const int result = system(R"sh([ "$(./echo -n hello; printf x)" = "$(printf '%s\nx' '-n hello')" ] && )sh"
                              R"sh([ "$(./echo -e hello; printf x)" = "$(printf '%s\nx' '-e hello')" ] && )sh"
                              R"sh([ "$(./echo -E hello; printf x)" = "$(printf '%s\nx' '-E hello')" ] && )sh"
                              R"sh([ "$(./echo -neE hello; printf x)" = "$(printf '%s\nx' '-neE hello')" ])sh");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(EchoIntegrationTest, OutputIsASingleLine)
{
    const int result = system("[ $(./echo a b | wc -l) -eq 1 ]");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(EchoIntegrationTest, WritesNothingToStderr)
{
    const int result = system(R"sh([ -z "$(./echo a b 2>&1 > /dev/null)" ])sh");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(EchoIntegrationTest, DoesNotReadStandardInput)
{
    const int result = system(R"sh([ "$(printf 'data\n' | (./echo a; cat); printf x)" = "$(printf 'a\ndata\nx')" ])sh");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}
