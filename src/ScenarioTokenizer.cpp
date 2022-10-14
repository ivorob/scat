#include "ScenarioTokenizer.h"

namespace {

enum class ParseState {
    Init,
    Id,
    StartDescription,
    Description,
    Spaces,
    Error,
};

void makeToken(ParseState state, std::string& tokenName, std::list<Scat::Token>& tokens) {
    if (tokenName.empty()) {
        return;
    }

    switch (state) {
        case ParseState::Id:
            tokens.push_back(
                Scat::Token(Scat::TokenId::Id, tokenName)
            );
            break;
        case ParseState::Description:
            tokens.push_back(
                Scat::Token(Scat::TokenId::Description, tokenName)
            );
            break;
        default:
            // Do nothing
            break;
    }
}

ParseState handleStreamSymbol(char currentSymbol, std::string& tokenName, std::list<Scat::Token>& tokens, ParseState state) {
    switch (state) {
        case ParseState::Init:
            if (std::isalpha(currentSymbol)) {
                tokenName.push_back(currentSymbol);
                return ParseState::Id;
            } 

            return ParseState::Error;
        case ParseState::Id:
            if (std::isalpha(currentSymbol)) {
                tokenName.push_back(currentSymbol);
            } else if (currentSymbol == ':') {
                makeToken(state, tokenName, tokens);

                tokenName.clear();
                return ParseState::StartDescription;
            }
            break;
        case ParseState::StartDescription:
            if (currentSymbol != ' ' && currentSymbol != '\t') {
                tokenName.push_back(currentSymbol);
                return ParseState::Description;
            }
            break;
        case ParseState::Description:
            if (currentSymbol == '\n') {
                auto it = tokenName.rbegin();
                while (it != tokenName.rend()) {
                    if (*it != '\r' && *it != ' ' && *it != '\t') {
                        break;
                    }

                    ++it;
                }

                tokenName.erase(std::prev(it).base());
                makeToken(state, tokenName, tokens);

                return ParseState::Spaces;
            } else {
                tokenName.push_back(currentSymbol);
            }
            break;
        case ParseState::Spaces:
            break;
        case ParseState::Error:
        default:
            throw std::runtime_error("Cannot parse scenario");
    }

    return state;
}

}

Scat::ScenarioTokenizer::ScenarioTokenizer(std::istream& input)
    : input(input) {

    input >> std::noskipws;
}

std::list<Scat::Token> Scat::ScenarioTokenizer::tokenize() {
    std::list<Scat::Token> tokens;

    ParseState state = ParseState::Init;
    char currentSymbol;
    std::string tokenName;
    while (input >> currentSymbol) {
        state = handleStreamSymbol(currentSymbol, tokenName, tokens, state);
    }

    makeToken(state, tokenName, tokens);
    return tokens;
}

