#include "ScenarioTokenizer.h"

namespace {

enum class ParseState {
    Init,
    Id,
    IdSpace,
    StartDescription,
    Description,
    Error,
};

class ParseContext {
public:
    void addSymbol(char currentSymbol);
    void changeState(ParseState newState);

    void handleStreamSymbol(char currentSymbol);
    void addToken();

    Scat::Tokens extractTokens();
private:
    void addToken(Scat::TokenId tokenId);
    void clearToken();
    void trimToken();

    ParseState getState() const;
private:
    std::string tokenName;
    Scat::Tokens tokens;
    ParseState state = ParseState::Init;
};

void ParseContext::addSymbol(char currentSymbol) {
    this->tokenName.push_back(currentSymbol);
}

void ParseContext::changeState(ParseState newState) {
    this->state = newState;
}

void ParseContext::addToken() {
    switch (this->state) {
        case ParseState::Id:
        case ParseState::IdSpace:
            addToken(Scat::TokenId::Id);
            break;
        case ParseState::Description:
            addToken(Scat::TokenId::Description);
            break;
        default:
            // Do nothing
            break;
    }
}

void ParseContext::addToken(Scat::TokenId tokenId) {
    if (!this->tokenName.empty()) {
        trimToken();

        this->tokens.push_back(
            Scat::Token(tokenId, this->tokenName)
        );

        clearToken();
    }
}

void ParseContext::clearToken() {
    this->tokenName.clear();
}

void ParseContext::trimToken() {
    auto it = tokenName.rbegin();
    while (it != tokenName.rend()) {
        if (*it != '\r' && *it != ' ' && *it != '\t') {
            break;
        }

        ++it;
    }

    tokenName.erase(it.base(), tokenName.end());
}

ParseState ParseContext::getState() const {
    return this->state;
}

Scat::Tokens ParseContext::extractTokens() {
    return std::move(this->tokens);
}

void ParseContext::handleStreamSymbol(char currentSymbol) {
    switch (getState()) {
        case ParseState::Init:
            if (std::isalpha(currentSymbol)) {
                addSymbol(currentSymbol);
                changeState(ParseState::Id);
            } else if (!std::isspace(currentSymbol)) {
                changeState(ParseState::Error);
            }

            break;
        case ParseState::Id:
            if (std::isalpha(currentSymbol)) {
                addSymbol(currentSymbol);
            } else if (currentSymbol == ':') {
                addToken();

                changeState(ParseState::StartDescription);
            } else if (std::isspace(currentSymbol)) {
                changeState(ParseState::IdSpace);
            }

            break;
        case ParseState::IdSpace:
            if (currentSymbol == ':') {
                addToken();

                changeState(ParseState::StartDescription);
            } else if (!std::isspace(currentSymbol)) {
                changeState(ParseState::Error);
            }
            break;
        case ParseState::StartDescription:
            if (currentSymbol != ' ' && currentSymbol != '\t') {
                addSymbol(currentSymbol);

                changeState(ParseState::Description);
            }
            break;
        case ParseState::Description:
            if (currentSymbol == '\n') {
                addToken();

                changeState(ParseState::Init);
            } else {
                addSymbol(currentSymbol);
            }
            break;
        case ParseState::Error:
        default:
            throw std::runtime_error("Cannot parse scenario");
    }
}

}

Scat::ScenarioTokenizer::ScenarioTokenizer(std::istream& input)
    : input(input) {

    input >> std::noskipws;
}

std::list<Scat::Token> Scat::ScenarioTokenizer::tokenize() {
    ParseContext parseContext;
    char currentSymbol;
    while (input >> currentSymbol) {
        parseContext.handleStreamSymbol(currentSymbol);
    }

    parseContext.addToken();
    return parseContext.extractTokens();
}

