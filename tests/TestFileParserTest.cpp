#include <gtest/gtest.h>
#include "TestFileParser.h"

TEST(TestFileParserTest, empty_stream_has_no_tests)
{
    std::stringstream input;
    Scat::TestFileParser testFileParser(input);

    auto testCommand = testFileParser.parseNext();

    ASSERT_FALSE(testCommand);
}
