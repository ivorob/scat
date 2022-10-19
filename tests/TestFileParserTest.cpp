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
        } catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
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

TEST(TestFileParserTest, valid_scenario_is_parsed_successfully) {
    // Arrange
    std::stringstream input(R"(Scenario: Install package
                               Given: Clean machine
                               When: Execute installation for package
                               Then: Packages files are present)");
    Scat::TestFileParser parser(input);

    // Act
    auto testScenario = parser.parseNext();

    // Assert
    ASSERT_TRUE(testScenario);
    ASSERT_EQ("Install package", testScenario->getDescription());
    ASSERT_EQ("Clean machine", testScenario->getGiven());
    ASSERT_EQ("Execute installation for package", testScenario->getWhen());
    ASSERT_EQ("Packages files are present", testScenario->getThen());
}

TEST(TestFileParserTest, scenario_with_command_is_parsed_successfully) {
    // Arrange
    std::stringstream input(R"(Scenario: Install package
                               Given: There is clean machine
                               When: #execute("install test.pkg -target /") is succeeded
                               Then: Packages files are present)");
    Scat::TestFileParser parser(input);

    // Act
    auto testScenario = parser.parseNext();

    // Assert
    ASSERT_TRUE(testScenario);
    ASSERT_EQ("Install package", testScenario->getDescription());
    ASSERT_EQ("There is clean machine", testScenario->getGiven());
    ASSERT_EQ(R"(#execute("install test.pkg -target /") is succeeded)", testScenario->getWhen());
    ASSERT_EQ("Packages files are present", testScenario->getThen());
}
