#include <gtest/gtest.h>

#include "ScenarioTokenizer.h"

TEST(ScenarioTokenizerTest, empty_stream_has_no_tokens) {
    // Arrange
    std::stringstream input;
    Scat::ScenarioTokenizer tokenizer(input);

    // Act
    auto tokens = tokenizer.tokenize();

    // Assert
    ASSERT_TRUE(tokens.empty());
}

TEST(ScenarioTokenizerTest, parsing_identifier_is_succeeded) {
    // Arrange
    std::stringstream input("someid: some description");
    Scat::ScenarioTokenizer tokenizer(input);

    // Act
    auto tokens = tokenizer.tokenize();

    // Assert
    ASSERT_EQ(2, tokens.size());

    const auto& firstToken = *(tokens.begin());
    ASSERT_EQ(Scat::TokenId::Id, firstToken.getId());
    ASSERT_EQ("someid", firstToken.getName());

    const auto& secondToken = *(std::next(tokens.begin()));
    ASSERT_EQ(Scat::TokenId::Description, secondToken.getId());
    ASSERT_EQ("some description", secondToken.getName());
}

TEST(ScenarioTokenizerTest, parsing_identifier_with_spaces_at_the_end_is_succeeded) {
    // Arrange
    std::stringstream input("someid \t:\t some description \t ");
    Scat::ScenarioTokenizer tokenizer(input);

    // Act
    auto tokens = tokenizer.tokenize();

    // Assert
    ASSERT_EQ(2, tokens.size());

    const auto& firstToken = *(tokens.begin());
    ASSERT_EQ(Scat::TokenId::Id, firstToken.getId());
    ASSERT_EQ("someid", firstToken.getName());

    const auto& secondToken = *(std::next(tokens.begin()));
    ASSERT_EQ(Scat::TokenId::Description, secondToken.getId());
    ASSERT_EQ("some description", secondToken.getName());
}

TEST(ScenarioTokenizerTest, parsing_identifier_with_spaces_at_the_beginning_is_succeeded) {
    // Arrange
    std::stringstream input("\t   someid \t:\t some description \t ");
    Scat::ScenarioTokenizer tokenizer(input);

    // Act
    auto tokens = tokenizer.tokenize();

    // Assert
    ASSERT_EQ(2, tokens.size());

    const auto& firstToken = *(tokens.begin());
    ASSERT_EQ(Scat::TokenId::Id, firstToken.getId());
    ASSERT_EQ("someid", firstToken.getName());

    const auto& secondToken = *(std::next(tokens.begin()));
    ASSERT_EQ(Scat::TokenId::Description, secondToken.getId());
    ASSERT_EQ("some description", secondToken.getName());
}

TEST(ScenarioTokenizerTest, parsing_identifier_with_new_line_symbol_is_succeeded) {
    // Arrange
    std::stringstream input("\n\nsomeid: some description \t \n\r\n\r\n");
    Scat::ScenarioTokenizer tokenizer(input);

    // Act
    auto tokens = tokenizer.tokenize();

    // Assert
    ASSERT_EQ(2, tokens.size());

    const auto& firstToken = *(tokens.begin());
    ASSERT_EQ(Scat::TokenId::Id, firstToken.getId());
    ASSERT_EQ("someid", firstToken.getName());

    const auto& secondToken = *(std::next(tokens.begin()));
    ASSERT_EQ(Scat::TokenId::Description, secondToken.getId());
    ASSERT_EQ("some description", secondToken.getName());
}

TEST(ScenarioTokenizerTest, parsing_a_few_identifier_is_succeeded) {
    // Arrange
    std::stringstream input("\n\nsomeid: some description \t \n\r\n"
                            "newid : new description");
    Scat::ScenarioTokenizer tokenizer(input);

    // Act
    auto tokens = tokenizer.tokenize();

    // Assert
    ASSERT_EQ(4, tokens.size());

    Scat::Tokens expectedTokens = {
        Scat::Token(Scat::TokenId::Id, "someid"),
        Scat::Token(Scat::TokenId::Description, "some description"),
        Scat::Token(Scat::TokenId::Id, "newid"),
        Scat::Token(Scat::TokenId::Description, "new description")
    };
    ASSERT_EQ(expectedTokens, tokens);
}

TEST(ScenarioTokenizerTest, parsing_description_with_command_is_succeeded) {
    // Arrange
    std::stringstream input("newid : some condition #execute(\"ls -l /tmp\") is ok");
    Scat::ScenarioTokenizer tokenizer(input);

    // Act
    auto tokens = tokenizer.tokenize();

    // Assert
    ASSERT_EQ(4, tokens.size());

    Scat::Tokens expectedTokens = {
        Scat::Token(Scat::TokenId::Id, "newid"),
        Scat::Token(Scat::TokenId::Description, "some condition "),
        Scat::Token(Scat::TokenId::Command, R"(execute("ls -l /tmp"))"),
        Scat::Token(Scat::TokenId::Description, " is ok")
    };

    ASSERT_EQ(expectedTokens, tokens);
}

TEST(ScenarioTokenizerTest, parsing_description_with_invalid_command_throw_exception) {
    // Arrange
    std::stringstream input("newid : some condition #execute is ok");
    Scat::ScenarioTokenizer tokenizer(input);

    // Act
    bool hasException = [&tokenizer]() -> bool {
        try {
            auto tokens = tokenizer.tokenize();
        } catch (...) {
            return true;
        }

        return false;
    }();

    // Assert
    ASSERT_TRUE(hasException);
}

// this test case will be handled not here
TEST(ScenarioTokenizerTest, parsing_command_with_invalid_arguments_is_succeeded) {
    // Arrange
    std::stringstream input("newid : some condition #execute(\"ls -l /tmp) is ok");
    Scat::ScenarioTokenizer tokenizer(input);

    // Act
    auto tokens = tokenizer.tokenize();

    // Assert
    ASSERT_EQ(4, tokens.size());

    Scat::Tokens expectedTokens = {
        Scat::Token(Scat::TokenId::Id, "newid"),
        Scat::Token(Scat::TokenId::Description, "some condition "),
        Scat::Token(Scat::TokenId::Command, R"(execute("ls -l /tmp))"),
        Scat::Token(Scat::TokenId::Description, " is ok")
    };

    ASSERT_EQ(expectedTokens, tokens);
}

TEST(ScenarioTokenizerTest, parsing_command_at_beginnig_of_description_is_succeeded) {
    // Arrange
    std::stringstream input(R"(Scenario: Install package
                               When: #execute("install test.pkg -target /") is succeeded)");
    Scat::ScenarioTokenizer tokenizer(input);

    // Act
    auto tokens = tokenizer.tokenize();

    // Assert
    ASSERT_EQ(5, tokens.size());

    Scat::Tokens expectedTokens = {
        Scat::Token(Scat::TokenId::Id, "Scenario"),
        Scat::Token(Scat::TokenId::Description, "Install package"),
        Scat::Token(Scat::TokenId::Id, "When"),
        Scat::Token(Scat::TokenId::Command, R"(execute("install test.pkg -target /"))"),
        Scat::Token(Scat::TokenId::Description, " is succeeded")
    };

    ASSERT_EQ(expectedTokens, tokens);

}
