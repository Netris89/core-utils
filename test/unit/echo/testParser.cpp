#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "parser.hpp"

using std::string;
using std::literals::string_literals::operator""s;

TEST(ParserTests, PlainText)
{
    string testedString = Parser::ParseArgument("NoEscapeSequencesHere");
    EXPECT_EQ(testedString, "NoEscapeSequencesHere");
    EXPECT_EQ(testedString.size(), 21);
}

TEST(ParserTests, EmptyInput)
{
    EXPECT_EQ(Parser::ParseArgument(""), "");
}

TEST(ParserTests, SpacesArePreserved)
{
    EXPECT_EQ(Parser::ParseArgument("a  b"), "a  b");
    EXPECT_EQ(Parser::ParseArgument("  lead and trail  "), "  lead and trail  ");
}

TEST(ParserTests, NonAsciiBytesArePassedThrough)
{
    EXPECT_EQ(Parser::ParseArgument("café"), "café");
}

TEST(ParserTests, CommandLineOptionLikeInput)
{
    EXPECT_EQ(Parser::ParseArgument("-eEcho"), "-eEcho");
    EXPECT_EQ(Parser::ParseArgument("-nNoNewline"), "-nNoNewline");
}

TEST(ParserTests, AlertSequence)
{
    EXPECT_EQ(Parser::ParseArgument("Alert\\aSound"), "Alert\aSound");
}

TEST(ParserTests, BackspaceSequence)
{
    EXPECT_EQ(Parser::ParseArgument("Back\\bSpace"), "Back\bSpace");
}

TEST(ParserTests, FormFeedSequence)
{
    EXPECT_EQ(Parser::ParseArgument("Form\\fFeed"), "Form\fFeed");
}

TEST(ParserTests, NewlineSequence)
{
    EXPECT_EQ(Parser::ParseArgument("New\\nLine"), "New\nLine");
}

TEST(ParserTests, TestTab)
{
    EXPECT_EQ(Parser::ParseArgument("Tab\\tSpace"), "Tab\tSpace");
}

TEST(ParserTests, CarriageReturnSequence)
{
    EXPECT_EQ(Parser::ParseArgument("Carriage\\rReturn"), "Carriage\rReturn");
}

TEST(ParserTests, VerticalTabSequence)
{
    EXPECT_EQ(Parser::ParseArgument("Vertical\\vTab"), "Vertical\vTab");
}

TEST(ParserTests, MixedEscapeSequences)
{
    EXPECT_EQ(Parser::ParseArgument("Mix\\a\\b\\t\\nEnd"), "Mix\a\b\t\nEnd");
}

TEST(ParserTests, EscapeAtStartAndEnd)
{
    EXPECT_EQ(Parser::ParseArgument("\\tTab"), "\tTab");
    EXPECT_EQ(Parser::ParseArgument("Tab\\t"), "Tab\t");
    EXPECT_EQ(Parser::ParseArgument("\\n"), "\n");
}

TEST(ParserTests, ConsecutiveEscapes)
{
    EXPECT_EQ(Parser::ParseArgument("\\n\\n\\n"), "\n\n\n");
}

TEST(ParserTests, InvalidEscape)
{
    EXPECT_EQ(Parser::ParseArgument("Invalid\\xEscape"), "Invalid\\xEscape");
    EXPECT_EQ(Parser::ParseArgument("\\q"), "\\q");
    EXPECT_EQ(Parser::ParseArgument("a\\ b"), "a\\ b");
}

TEST(ParserTests, BackslashSequence)
{
    EXPECT_EQ(Parser::ParseArgument("Back\\slash"), "Back\\slash");
    EXPECT_EQ(Parser::ParseArgument("EndWith\\"), "EndWith\\");
    EXPECT_EQ(Parser::ParseArgument("\\StartWith"), "\\StartWith");
}

TEST(ParserTests, LoneBackslash)
{
    EXPECT_EQ(Parser::ParseArgument("\\"), "\\");
}

TEST(ParserTests, EscapeBeforeTrailingLoneBackslash)
{
    EXPECT_EQ(Parser::ParseArgument("Tab\\tEnd\\"), "Tab\tEnd\\");
}

TEST(ParserTests, MultipleEscapesBeforeTrailingLoneBackslash)
{
    EXPECT_EQ(Parser::ParseArgument("A\\nB\\tC\\"), "A\nB\tC\\");
}

TEST(ParserTests, EscapedBackslashAtEnd)
{
    EXPECT_EQ(Parser::ParseArgument("A\\\\"), "A\\");
}

TEST(ParserTests, EscapedBackslashThenB)
{
    EXPECT_EQ(Parser::ParseArgument("a\\\\b"), "a\\b");
}

TEST(ParserTests, EscapedBackslashThenN)
{
    EXPECT_EQ(Parser::ParseArgument("a\\\\nb"), "a\\nb");
}

TEST(ParserTests, ThreeBackslashes)
{
    // One escaped backslash followed by a lone trailing one
    EXPECT_EQ(Parser::ParseArgument("\\\\\\"), "\\\\");
}

TEST(ParserTests, FourBackslashes)
{
    // Two escaped backslashes
    EXPECT_EQ(Parser::ParseArgument("\\\\\\\\"), "\\\\");
}

TEST(ParserTests, EscapedBackslashThenEscape)
{
    // One escaped backslash, then a real newline escape
    EXPECT_EQ(Parser::ParseArgument("\\\\\\n"), "\\\n");
}

TEST(ParserTests, SuppressNewline)
{
    EXPECT_EQ(Parser::ParseArgument("Suppress\\cNewline"), "Suppress");
}

TEST(ParserTests, SuppressNewlineAtStart)
{
    EXPECT_EQ(Parser::ParseArgument("\\cabc"), "");
}

TEST(ParserTests, SuppressNewlineAtEnd)
{
    EXPECT_EQ(Parser::ParseArgument("abc\\c"), "abc");
}

TEST(ParserTests, EscapesAfterSuppressNewlineAreIgnored)
{
    EXPECT_EQ(Parser::ParseArgument("a\\cb\\nc"), "a");
}

TEST(ParserTests, EscapesBeforeSuppressNewlineAreKept)
{
    EXPECT_EQ(Parser::ParseArgument("a\\nb\\cc"), "a\nb");
}

TEST(ParserTests, EscapedBackslashBeforeCIsNotSuppression)
{
    // An escaped backslash followed by the letter c is plain text, not \c
    EXPECT_EQ(Parser::ParseArgument("a\\\\cb"), "a\\cb");
}

TEST(ParserTests, OctalEscape)
{
    EXPECT_EQ(Parser::ParseArgument("\\041"), "!");
    EXPECT_EQ(Parser::ParseArgument("\\012"), "\n");
    EXPECT_EQ(Parser::ParseArgument("\\078"), "\a8"); // 8 is not an octal digit, so it stays as text
}

TEST(ParserTests, OctalEscapeWithoutDigitsIsNul)
{
    EXPECT_EQ(Parser::ParseArgument("\\0"), "\0"s);
    EXPECT_EQ(Parser::ParseArgument("\\0x"), "\0x"s);
    EXPECT_EQ(Parser::ParseArgument("a\\0b"), "a\0b"s);
    EXPECT_EQ(Parser::ParseArgument("\\08"), "\0"
                                             "8"s);
}

TEST(ParserTests, OctalEscapeOneDigit)
{
    EXPECT_EQ(Parser::ParseArgument("\\01"), "\x01");
    EXPECT_EQ(Parser::ParseArgument("\\07"), "\a");
}

TEST(ParserTests, OctalEscapeThreeDigits)
{
    EXPECT_EQ(Parser::ParseArgument("\\0101"), "A");
    EXPECT_EQ(Parser::ParseArgument("x\\0101y"), "xAy");
    EXPECT_EQ(Parser::ParseArgument("\\0101\\0102"), "AB");
}

TEST(ParserTests, OctalEscapeReadsAtMostThreeDigits)
{
    EXPECT_EQ(Parser::ParseArgument("\\01234"), "S4");
    EXPECT_EQ(Parser::ParseArgument("\\0101234"), "A234");
}

TEST(ParserTests, OctalEscapeStopsAtNonOctalDigit)
{
    EXPECT_EQ(Parser::ParseArgument("\\0128"), "\n8");
    EXPECT_EQ(Parser::ParseArgument("\\0189"), "\x01"
                                               "89");
}

TEST(ParserTests, OctalEscapeFollowedByNonAsciiByte)
{
    EXPECT_EQ(Parser::ParseArgument("\\0é"), "\0é"s);
}

TEST(ParseArgumentsTests, NoArgumentsWritesOnlyNewline)
{
    EXPECT_EQ(Parser::ParseArguments({}), "\n");
}

TEST(ParseArgumentsTests, SingleArgument)
{
    EXPECT_EQ(Parser::ParseArguments({"a"}), "a\n");
}

TEST(ParseArgumentsTests, ArgumentsAreSeparatedBySingleSpaces)
{
    EXPECT_EQ(Parser::ParseArguments({"a", "b", "c"}), "a b c\n");
}

TEST(ParseArgumentsTests, EmptyArgument)
{
    EXPECT_EQ(Parser::ParseArguments({""}), "\n");
}

TEST(ParseArgumentsTests, EmptyArgumentsKeepTheirSeparators)
{
    EXPECT_EQ(Parser::ParseArguments({"", ""}), " \n");
    EXPECT_EQ(Parser::ParseArguments({"a", "", "b"}), "a  b\n");
}

TEST(ParseArgumentsTests, InternalSpacesArePreserved)
{
    EXPECT_EQ(Parser::ParseArguments({"a  b", "c"}), "a  b c\n");
}

TEST(ParseArgumentsTests, EscapesAreInterpretedInEveryArgument)
{
    EXPECT_EQ(Parser::ParseArguments({"a\\tb", "c\\nd"}), "a\tb c\nd\n");
}

TEST(ParseArgumentsTests, EscapedNewlineThenFinalNewline)
{
    EXPECT_EQ(Parser::ParseArguments({"a\\n"}), "a\n\n");
}

TEST(ParseArgumentsTests, OptionLikeOperandsAreText)
{
    EXPECT_EQ(Parser::ParseArguments({"-n", "foo"}), "-n foo\n");
    EXPECT_EQ(Parser::ParseArguments({"-e", "foo"}), "-e foo\n");
    EXPECT_EQ(Parser::ParseArguments({"-E", "foo"}), "-E foo\n");
    EXPECT_EQ(Parser::ParseArguments({"--", "foo"}), "-- foo\n");
    EXPECT_EQ(Parser::ParseArguments({"-x"}), "-x\n");
}

TEST(ParseArgumentsTests, TrailingBackslashDoesNotAffectNextArgument)
{
    // The lone backslash at the end of an argument is literal, it must not escape the separator
    EXPECT_EQ(Parser::ParseArguments({"a\\", "b"}), "a\\ b\n");
}

TEST(ParseArgumentsTests, SuppressNewlineEndsTheOutput)
{
    EXPECT_EQ(Parser::ParseArguments({"a\\c"}), "a");
    EXPECT_EQ(Parser::ParseArguments({"a\\cb"}), "a");
    EXPECT_EQ(Parser::ParseArguments({"\\c"}), "");
}

TEST(ParseArgumentsTests, SuppressNewlineIgnoresFollowingArguments)
{
    EXPECT_EQ(Parser::ParseArguments({"a\\cb", "c"}), "a");
    EXPECT_EQ(Parser::ParseArguments({"x", "a\\cb", "c"}), "x a");
}
