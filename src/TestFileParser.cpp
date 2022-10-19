#include <unordered_set>
#include <unordered_map>

#include "TestFileParser.h"
#include "TestFileParseException.h"
#include "ScenarioTokenizer.h"

namespace {

std::unordered_set<std::string> availableTokens = { "Scenario", "Given", "When", "Then" };

std::unordered_map<Scat::TokenId, std::unordered_set<Scat::TokenId>> tokensTransitionMap = {
    { Scat::TokenId::None, { Scat::TokenId::Id } },
    { Scat::TokenId::Id, { Scat::TokenId::Command, Scat::TokenId::Description } },
    { Scat::TokenId::Description, { Scat::TokenId::Command, Scat::TokenId::Id } },
    { Scat::TokenId::Command, { Scat::TokenId::Description } }
};

}

Scat::TestFileParser::TestFileParser(std::istream& input)
    : input(input)
{
}

std::optional<Scat::TestScenario> Scat::TestFileParser::parseNext()
{
    std::unordered_map<std::string, std::string> scenario;

    try {
        Scat::ScenarioTokenizer tokenizer(input);

        std::string idName;
        auto lastTokenId = Scat::TokenId::None;
        auto tokens = tokenizer.tokenize();
        for (const auto& token : tokens) {
            if (!isValidToken(token.getId(), lastTokenId)) {
                throw TestFileParseException(1, 0, "Unexpected token"); // TODO: change to valid exception
            }

            switch (token.getId()) {
                case Scat::TokenId::Id:
                    if (availableTokens.count(token.getName()) != 0) {
                        idName = token.getName();
                    } else {
                        throw TestFileParseException(1, 0, "Unexpected token"); // TODO: change to valid exception
                    }
                    break;
                case Scat::TokenId::Command:
                    scenario[idName] += "#";

                    [[fallthrough]];
                case Scat::TokenId::Description:
                    scenario[idName] += token.getName();
                    break;
                default:
                    throw TestFileParseException(1, 0, "Invalid token id"); // TODO: change to valid exception
            }

            lastTokenId = token.getId();
        }
    } catch (const std::exception&) {
        throw TestFileParseException(1, 0, "invalid scenario"); // TODO: change to valid exception
    }

    if (!scenario.empty()) {
        return Scat::TestScenario(
                    scenario.at("Scenario"),
                    scenario.at("Given"),
                    scenario.at("When"),
                    scenario.at("Then")
                );
    }

    return std::nullopt;
}

bool Scat::TestFileParser::isValidToken(
        Scat::TokenId currentTokenId,
        Scat::TokenId lastTokenId) const {
    auto it = tokensTransitionMap.find(lastTokenId);
    return it != std::cend(tokensTransitionMap) &&
        it->second.count(currentTokenId);
}
