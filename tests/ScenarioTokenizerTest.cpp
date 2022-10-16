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
