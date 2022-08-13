#include <gtest/gtest.h>
#include <optional>

#include "TestFileParser.h"
#include "TestFileParseException.h"

TEST(TestFileParserTest, empty_stream_has_no_scenarios)
{
    std::stringstream input;
    Scat::TestFileParser testFileParser(input);

    auto testScenario = testFileParser.parseNext();

    ASSERT_FALSE(testScenario);
}

TEST(TestFileParserTest, parse_invalid_scenario_generates_exception)
{
    std::stringstream input("invalid scenario\nnew line");
    Scat::TestFileParser testFileParser(input);

    auto exception = [&testFileParser]() -> std::optional<Scat::TestFileParseException> {
        try {
            auto testScenario = testFileParser.parseNext();
        } catch (const Scat::TestFileParseException& e) {
            return e;
        }

        return std::nullopt;
    }();

    ASSERT_TRUE(exception);
    ASSERT_EQ(1, exception->getLineNumber());
    ASSERT_EQ(0, exception->getSymbolIndex());
    ASSERT_EQ("invalid scenario", exception->getText());
    ASSERT_EQ(
        std::string("Parse exception on line 1 in 0 position. Expectation: Scenario block. "
            "Text: \"invalid scenario\"."), 
        exception->what()
    );
}
