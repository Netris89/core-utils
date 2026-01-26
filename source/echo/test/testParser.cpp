#include <gtest/gtest.h>
#include <string>

#include "parser.hpp"

using std::string;

TEST(ParserTests, PlainText)
{
    string testedString = Parser::ParseArgument("NoEscapeSequencesHere");
    EXPECT_EQ(testedString, "NoEscapeSequencesHere");
    EXPECT_EQ(testedString.size(), 21);
}

TEST(ParserTests, AlertSequence)
{
    EXPECT_EQ(Parser::ParseArgument("Alert\aSound"), "Alert\aSound");
}

TEST(ParserTests, BackspaceSequence)
{
    EXPECT_EQ(Parser::ParseArgument("Back\bSpace"), "Back\bSpace");
}

TEST(ParserTests, SuppressNewline)
{
    EXPECT_EQ(Parser::ParseArgument("Suppress\\cNewline"), "Suppress");
}

TEST(ParserTests, FormFeedSequence)
{
    EXPECT_EQ(Parser::ParseArgument("Form\fFeed"), "Form\fFeed");
}

TEST(ParserTests, NewlineSequence)
{
    EXPECT_EQ(Parser::ParseArgument("New\nLine"), "New\nLine");
}

TEST(ParserTests, TestTab)
{
    EXPECT_EQ(Parser::ParseArgument("Tab\tSpace"), "Tab\tSpace");
}

TEST(ParserTests, CarriageReturnSequence)
{
    EXPECT_EQ(Parser::ParseArgument("Carriage\rReturn"), "Carriage\rReturn");
}

TEST(ParserTests, VerticalTabSequence)
{
    EXPECT_EQ(Parser::ParseArgument("Vertical\vTab"), "Vertical\vTab");
}

TEST(ParserTests, BackslashSequence)
{
    EXPECT_EQ(Parser::ParseArgument("Back\\slash"), "Back\\slash");
    EXPECT_EQ(Parser::ParseArgument("EndWith\\"), "EndWith\\");
    EXPECT_EQ(Parser::ParseArgument("\\StartWith"), "\\StartWith");
}

TEST(ParserTests, InvalidEscape)
{
    EXPECT_EQ(Parser::ParseArgument("Invalid\\xEscape"), "Invalid\\xEscape");
}

TEST(ParserTests, EmptyInput)
{
    EXPECT_EQ(Parser::ParseArgument(""), "");
}

TEST(ParserTests, OctalEscape)
{
    EXPECT_EQ(Parser::ParseArgument("\\041"), "!");
    EXPECT_EQ(Parser::ParseArgument("\\012"), "\n");
    EXPECT_EQ(Parser::ParseArgument("\\078"), "\078");
    EXPECT_EQ(Parser::ParseArgument("\\0x"), "\\0x");
}

TEST(ParserTests, CommandLineOptionLikeInput)
{
    EXPECT_EQ(Parser::ParseArgument("-eEcho"), "-eEcho");
    EXPECT_EQ(Parser::ParseArgument("-nNoNewline"), "-nNoNewline");
}

TEST(ParserTests, MixedEscapeSequences)
{
    EXPECT_EQ(Parser::ParseArgument("Mix\\a\\b\\t\\nEnd"), string("Mix") + '\a' + '\b' + '\t' + '\n' + "End");
}
