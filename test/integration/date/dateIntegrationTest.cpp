#include <cstdlib>

#include <gtest/gtest.h>
#include <sys/wait.h>

using std::system;

TEST(DateIntegrationTest, ReturnsZero)
{
    const int result = system("./date > /dev/null");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(DateIntegrationTest, OutputIsASingleLine)
{
    const int result = system("[ $(./date | wc -l) -eq 1 ]");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(DateIntegrationTest, DefaultFormatInPosixLocale)
{
    // Same layout as +"%a %b %e %H:%M:%S %Z %Y", the time zone being forced so that %Z is predictable
    const int result = system(R"sh(TZ=UTC0 LC_ALL=C ./date | grep -Eq '^[A-Z][a-z]{2} [A-Z][a-z]{2} [ 0-9][0-9] [0-9]{2}:[0-9]{2}:[0-9]{2} UTC [0-9]{4}$')sh");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(DateIntegrationTest, FormatTextIsCopiedWithoutChange)
{
    const int result = system(R"sh([ "$(./date '+hello world'; printf x)" = "$(printf '%s\nx' 'hello world')" ])sh");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(DateIntegrationTest, EmptyFormatWritesNewline)
{
    const int result = system(R"sh([ "$(./date +; printf x)" = "$(printf '\nx')" ])sh");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(DateIntegrationTest, PercentSpecifierWritesPercentSign)
{
    const int result = system(R"sh([ "$(./date '+%%'; printf x)" = "$(printf '%s\nx' '%')" ])sh");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(DateIntegrationTest, NSpecifierWritesNewline)
{
    const int result = system(R"sh([ "$(./date '+a%nb'; printf x)" = "$(printf 'a\nb\nx')" ])sh");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(DateIntegrationTest, TSpecifierWritesTab)
{
    const int result = system(R"sh([ "$(./date '+a%tb'; printf x)" = "$(printf 'a\tb\nx')" ])sh");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

// Each equivalence is checked in a single invocation, so the clock cannot change between the two values

TEST(DateIntegrationTest, DSpecifierEqualsMdy)
{
    const int result = system(R"sh(out=$(./date '+%D|%m/%d/%y'); [ "${out%|*}" = "${out#*|}" ])sh");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(DateIntegrationTest, TSpecifierEqualsHms)
{
    const int result = system(R"sh(out=$(./date '+%T|%H:%M:%S'); [ "${out%|*}" = "${out#*|}" ])sh");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(DateIntegrationTest, HSpecifierEqualsB)
{
    const int result = system(R"sh(out=$(LC_ALL=C ./date '+%h|%b'); [ "${out%|*}" = "${out#*|}" ])sh");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(DateIntegrationTest, RSpecifierEqualsIMSp)
{
    const int result = system(R"sh(out=$(LC_ALL=C ./date '+%r|%I:%M:%S %p'); [ "${out%|*}" = "${out#*|}" ])sh");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(DateIntegrationTest, LowerXSpecifierEqualsMdyInPosixLocale)
{
    const int result = system(R"sh(out=$(LC_ALL=C ./date '+%x|%m/%d/%y'); [ "${out%|*}" = "${out#*|}" ])sh");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(DateIntegrationTest, UpperXSpecifierEqualsHmsInPosixLocale)
{
    const int result = system(R"sh(out=$(LC_ALL=C ./date '+%X|%H:%M:%S'); [ "${out%|*}" = "${out#*|}" ])sh");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

// The failing specifier is written to stderr by the loops below

TEST(DateIntegrationTest, NamesInPosixLocale)
{
    const int result = system(R"sh(export LC_ALL=C; )sh"
                              R"sh(for t in )sh"
                              R"sh('%a:^(Mon|Tue|Wed|Thu|Fri|Sat|Sun)$' )sh"
                              R"sh('%A:^(Monday|Tuesday|Wednesday|Thursday|Friday|Saturday|Sunday)$' )sh"
                              R"sh('%b:^(Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec)$' )sh"
                              R"sh('%B:^(January|February|March|April|May|June|July|August|September|October|November|December)$' )sh"
                              R"sh('%p:^(AM|PM)$'; do )sh"
                              R"sh(f=${t%%:*}; r=${t#*:}; )sh"
                              R"sh(./date "+$f" | grep -Eq "$r" || { echo "failing specifier: $f" >&2; exit 1; }; )sh"
                              R"sh(done)sh");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(DateIntegrationTest, NumericFieldsAreInTheirRange)
{
    const int result = system(R"sh(export LC_ALL=C; )sh"
                              R"sh(for t in )sh"
                              R"sh('%Y:^[0-9]{4}$' '%y:^[0-9]{2}$' '%C:^[0-9]{2}$' )sh"
                              R"sh('%m:^(0[1-9]|1[0-2])$' )sh"
                              R"sh('%d:^(0[1-9]|[12][0-9]|3[01])$' )sh"
                              R"sh('%e:^( [1-9]|[12][0-9]|3[01])$' )sh"
                              R"sh('%H:^([01][0-9]|2[0-3])$' )sh"
                              R"sh('%I:^(0[1-9]|1[0-2])$' )sh"
                              R"sh('%M:^[0-5][0-9]$' '%S:^([0-5][0-9]|60)$' )sh"
                              R"sh('%j:^[0-9]{3}$' '%u:^[1-7]$' '%w:^[0-6]$' )sh"
                              R"sh('%U:^([0-4][0-9]|5[0-3])$' '%V:^(0[1-9]|[1-4][0-9]|5[0-3])$'; do )sh"
                              R"sh(f=${t%%:*}; r=${t#*:}; )sh"
                              R"sh(./date "+$f" | grep -Eq "$r" || { echo "failing specifier: $f" >&2; exit 1; }; )sh"
                              R"sh(done)sh");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(DateIntegrationTest, LocalDateMatchesSystemDate)
{
    // Fails only if midnight passes between the two calls
    const int result = system(R"sh([ "$(./date +%Y-%m-%d)" = "$(date +%Y-%m-%d)" ])sh");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(DateIntegrationTest, UtcDateMatchesSystemDate)
{
    // Fails only if midnight UTC passes between the two calls
    const int result = system(R"sh([ "$(./date -u +%Y-%m-%d)" = "$(date -u +%Y-%m-%d)" ])sh");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(DateIntegrationTest, OptionUWritesUtcTimeZoneName)
{
    const int result = system(R"sh(./date -u +%Z | grep -Eq '^(UTC|GMT)$')sh");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(DateIntegrationTest, OptionUIgnoresTzVariable)
{
    const int result = system(R"sh(TZ=EST5 ./date -u +%Z | grep -Eq '^(UTC|GMT)$')sh");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(DateIntegrationTest, TzVariableSelectsTimeZone)
{
    const int result = system(R"sh([ "$(TZ=EST5 ./date +%Z)" = "EST" ])sh");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(DateIntegrationTest, OptionUAcceptsFormat)
{
    const int result = system(R"sh(./date -u +%Y | grep -Eq '^[0-9]{4}$')sh");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(DateIntegrationTest, DoubleDashEndsOptions)
{
    const int result = system(R"sh(./date -- +%Y | grep -Eq '^[0-9]{4}$')sh");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(DateIntegrationTest, UnknownOptionReturnsError)
{
    const int result = system("./date -z > /dev/null 2>&1");

    EXPECT_GT(WEXITSTATUS(result), 0);
}

TEST(DateIntegrationTest, UnknownOptionWritesNothingToStdout)
{
    const int result = system(R"sh([ -z "$(./date -z 2>/dev/null)" ])sh");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(DateIntegrationTest, UnknownOptionWritesDiagnosticToStderr)
{
    const int result = system(R"sh([ -n "$(./date -z 2>&1 > /dev/null)" ])sh");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}

TEST(DateIntegrationTest, InvalidOperandReturnsError)
{
    const int result = system("./date foo > /dev/null 2>&1");

    EXPECT_GT(WEXITSTATUS(result), 0);
}

TEST(DateIntegrationTest, TwoOperandsReturnsError)
{
    const int result = system("./date +%Y +%Y > /dev/null 2>&1");

    EXPECT_GT(WEXITSTATUS(result), 0);
}
