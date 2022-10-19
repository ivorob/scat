#pragma once

#include <istream>
#include <optional>

#include "TestScenario.h"
#include "Token.h"

namespace Scat {

class TestFileParser {
public:
    TestFileParser(std::istream& input);

    std::optional<TestScenario> parseNext();
private:
    bool isValidToken(
            Scat::TokenId currentTokenId,
            Scat::TokenId lastTokenId) const;
private:
    std::istream& input;
};

}
