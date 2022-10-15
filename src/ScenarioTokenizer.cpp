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

class ParseContext {
public:
    std::string tokenName;
public:
    void addSymbol(char currentSymbol);
    void changeState(ParseState newState);

    void handleStreamSymbol(char currentSymbol);
    void addToken();

    const Scat::Tokens& getTokens() const;
private:
    void addToken(Scat::TokenId tokenId);
    void clearToken();

    ParseState getState() const;
private:
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
        this->tokens.push_back(
            Scat::Token(tokenId, this->tokenName)
        );

        clearToken();
    }
}

void ParseContext::clearToken() {
    this->tokenName.clear();
}

ParseState ParseContext::getState() const {
    return this->state;
}

const Scat::Tokens& ParseContext::getTokens() const {
    return this->tokens;
}

void ParseContext::handleStreamSymbol(char currentSymbol) {
    switch (getState()) {
        case ParseState::Init:
            if (std::isalpha(currentSymbol)) {
                addSymbol(currentSymbol);
                changeState(ParseState::Id);
            } else {
                changeState(ParseState::Error);
            }

            break;
        case ParseState::Id:
            if (std::isalpha(currentSymbol)) {
                addSymbol(currentSymbol);
            } else if (currentSymbol == ':') {
                addToken();

                changeState(ParseState::StartDescription);
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
                auto it = tokenName.rbegin();
                while (it != tokenName.rend()) {
                    if (*it != '\r' && *it != ' ' && *it != '\t') {
                        break;
                    }

                    ++it;
                }

                tokenName.erase(std::prev(it).base());
                addToken();

                changeState(ParseState::Spaces);
            } else {
                addSymbol(currentSymbol);
            }
            break;
        case ParseState::Spaces:
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
    return parseContext.getTokens();
}

