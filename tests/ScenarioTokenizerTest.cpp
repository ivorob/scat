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
