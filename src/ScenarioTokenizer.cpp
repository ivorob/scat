#include <functional>

#include "ScenarioTokenizer.h"

namespace {

enum class ParseState {
    Init,
    Id,
    IdSpace,
    StartDescription,
    Description,
    Command,
    CommandArguments,
    Error,
};

static std::function<void()> emptyFunction = []{};

class ParseContext {
public:
    void addSymbol(char currentSymbol);
    void changeState(ParseState newState);

    void handleStreamSymbol(char currentSymbol);
    void addTokenRightTrim();
    void addToken(std::function<void()> preprocess = emptyFunction);

    Scat::Tokens extractTokens();
private:
    void addToken(Scat::TokenId tokenId, std::function<void()> preprocess);
    void clearToken();
    void rightTrimToken();

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

void ParseContext::addTokenRightTrim() {
    addToken(std::bind(&ParseContext::rightTrimToken, this));
}

void ParseContext::addToken(std::function<void()> preprocess) {
    switch (this->state) {
        case ParseState::Id:
        case ParseState::IdSpace:
            addToken(Scat::TokenId::Id, preprocess);
            break;
        case ParseState::Description:
            addToken(Scat::TokenId::Description, preprocess);
            break;
        case ParseState::CommandArguments:
            addToken(Scat::TokenId::Command, preprocess);
            break;
        default:
            // Do nothing
            break;
    }
}

void ParseContext::addToken(Scat::TokenId tokenId, std::function<void()> preprocess) {
    if (!this->tokenName.empty()) {
        preprocess();

        this->tokens.push_back(
            Scat::Token(tokenId, this->tokenName)
        );

        clearToken();
    }
}

void ParseContext::clearToken() {
    this->tokenName.clear();
}

void ParseContext::rightTrimToken() {
    tokenName.erase(std::find_if(tokenName.rbegin(), tokenName.rend(), [](char letter) {
        return letter != '\r' && letter != ' ' && letter != '\t';
    }).base(), tokenName.end());
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
            if (currentSymbol == '#') {
                addToken();

                changeState(ParseState::Command);
            } else if (currentSymbol != ' ' && currentSymbol != '\t') {
                addSymbol(currentSymbol);

                changeState(ParseState::Description);
            }
            break;
        case ParseState::Description:
            if (currentSymbol == '\n') {
                addTokenRightTrim();

                changeState(ParseState::Init);
            } else if (currentSymbol == '#') {
                addToken();

                changeState(ParseState::Command);
            } else {
                addSymbol(currentSymbol);
            }
            break;
        case ParseState::Command:
            if (std::isalnum(currentSymbol)) {
                addSymbol(currentSymbol);
            } else if (currentSymbol == '(') {
                addSymbol(currentSymbol);

                changeState(ParseState::CommandArguments);
            } else {
                changeState(ParseState::Error);
            }
            break;
        case ParseState::CommandArguments:
            if (currentSymbol == '\n') {
                changeState(ParseState::Error);
            } else if (currentSymbol == ')') {
                addSymbol(currentSymbol);

                addToken();
                changeState(ParseState::Description);
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

    parseContext.addTokenRightTrim();
    return parseContext.extractTokens();
}

