#include <unordered_set>
#include <unordered_map>

#include "TestFileParser.h"
#include "TestFileParseException.h"
#include "ScenarioTokenizer.h"

namespace {

std::unordered_set<std::string> availableTokens = { "Scenario", "Given", "When", "Then" };

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

        auto expectedId = Scat::TokenId::Id;
        std::string idName;
        auto tokens = tokenizer.tokenize();
        for (const auto& token : tokens) {
            if (token.getId() != expectedId) {
                throw TestFileParseException(1, 0, "Unexpected token"); // TODO: change to valid exception
            }

            if (token.getId() == Scat::TokenId::Id) {
                if (availableTokens.count(token.getName()) != 0) {
                    idName = token.getName();
                } else {
                    throw TestFileParseException(1, 0, "Unexpected token"); // TODO: change to valid exception
                }

                expectedId = Scat::TokenId::Description;
            } else if (token.getId() == Scat::TokenId::Description && !idName.empty()) {
                scenario[idName] = token.getName();

                idName.clear();
                expectedId = Scat::TokenId::Id;
            }
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
