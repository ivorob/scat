#include <gtest/gtest.h>
#include "TestFileParser.h"

TEST(TestFileParserTest, empty_stream_has_no_tests)
{
    std::stringstream input;
    Scat::TestFileParser testFileParser(input);

    auto testCommand = testFileParser.parseNext();

    ASSERT_FALSE(testCommand);
}

TEST(TestFileParserTest, parse_command_without_arguments_is_succeeded)
{
    std::stringstream input("start");
    Scat::TestFileParser testFileParser(input);

    auto testCommand = testFileParser.parseNext();

    ASSERT_TRUE(testCommand);
    ASSERT_EQ("start", testCommand->getName());
}

TEST(TestFileParserTest, parse_command_with_arguments_is_succeeded)
{
    std::stringstream input("include testfile 10");
    Scat::TestFileParser testFileParser(input);

    auto testCommand = testFileParser.parseNext();

    ASSERT_TRUE(testCommand);
    ASSERT_EQ("include", testCommand->getName());
    Scat::TestCommand::Arguments expectedArguments { "testfile", "10" };
    ASSERT_EQ(expectedArguments, testCommand->getArguments());
}

TEST(TestFileParserTest, parse_command_with_one_argument_is_succeeded)
{
    std::stringstream input("include       testfile        ");
    Scat::TestFileParser testFileParser(input);

    auto testCommand = testFileParser.parseNext();

    ASSERT_TRUE(testCommand);
    ASSERT_EQ("include", testCommand->getName());
    Scat::TestCommand::Arguments expectedArguments { "testfile" };
    ASSERT_EQ(expectedArguments, testCommand->getArguments());
}

TEST(TestFileParserTest, line_with_spaces_isnot_interpret_as_command)
{
    std::stringstream input("\n\n     \ninclude       testfile        ");
    Scat::TestFileParser testFileParser(input);

    auto testCommand = testFileParser.parseNext();

    ASSERT_TRUE(testCommand);
    ASSERT_EQ("include", testCommand->getName());
    Scat::TestCommand::Arguments expectedArguments { "testfile" };
    ASSERT_EQ(expectedArguments, testCommand->getArguments());
}
