#include <functional>
#include <unordered_map>

#include "ScenarioTokenizer.h"

namespace {

class AbstractParseState {
public:
    virtual ~AbstractParseState() = default;

    virtual void process(char symbol) = 0;
    virtual void addToken(std::function<void(std::string&)>) {}
};

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

std::function<void(std::string&)> emptyFunction = [](std::string&){};
std::function<void(std::string&)> rightTrimToken = 
    [](std::string& tokenName) {
        tokenName.erase(std::find_if(tokenName.rbegin(), tokenName.rend(), [](char letter) {
            return letter != '\r' && letter != ' ' && letter != '\t';
        }).base(), tokenName.end());
    };

class ParseContext {
public:
    ParseContext();

    void addSymbol(char currentSymbol);
    void changeState(ParseState newState);

    void handleStreamSymbol(char currentSymbol);
    void addToken(Scat::TokenId tokenId, 
            std::function<void(std::string&)> preprocess = emptyFunction);
    void addTokenRightTrim();

    Scat::Tokens extractTokens();
private:
    void clearToken();
private:
    using ParseStateHolder = std::shared_ptr<AbstractParseState>;
private:
    ParseStateHolder getState();
private:
    std::string tokenName;
    Scat::Tokens tokens;
    std::shared_ptr<AbstractParseState> state;
    std::unordered_map<ParseState, ParseStateHolder> states;
};

template <typename T>
std::shared_ptr<AbstractParseState> createStateFunction(ParseContext& parseContext) {
    std::shared_ptr<AbstractParseState> state(new T(parseContext));
    return state;
}

class InitState : public AbstractParseState {
public:
    InitState(ParseContext& parseContext)
        : parseContext(parseContext) {
    }

    void process(char symbol) override {
        if (std::isalpha(symbol)) {
            parseContext.addSymbol(symbol);
            parseContext.changeState(ParseState::Id);
        } else if (!std::isspace(symbol)) {
            parseContext.changeState(ParseState::Error);
        }
    }
private:
    ParseContext& parseContext;
};

class IdState : public AbstractParseState {
public:
    IdState(ParseContext& parseContext)
        : parseContext(parseContext) {
    }

    void process(char symbol) override {
        if (std::isalpha(symbol)) {
            parseContext.addSymbol(symbol);
        } else if (symbol == ':') {
            parseContext.addToken(Scat::TokenId::Id);

            parseContext.changeState(ParseState::StartDescription);
        } else if (std::isspace(symbol)) {
            parseContext.changeState(ParseState::IdSpace);
        }
    }

    void addToken(std::function<void(std::string&)> preprocess) override {
        parseContext.addToken(Scat::TokenId::Id, preprocess);
    }
private:
    ParseContext& parseContext;
};

class IdSpaceState : public AbstractParseState {
public:
    IdSpaceState(ParseContext& parseContext)
        : parseContext(parseContext) {
    }

    void process(char symbol) override {
        if (symbol == ':') {
            parseContext.addToken(Scat::TokenId::Id);

            parseContext.changeState(ParseState::StartDescription);
        } else if (!std::isspace(symbol)) {
            parseContext.changeState(ParseState::Error);
        }
    }

    void addToken(std::function<void(std::string&)> preprocess) override {
        parseContext.addToken(Scat::TokenId::Id, preprocess);
    }
private:
    ParseContext& parseContext;
};

class StartDescriptionState : public AbstractParseState {
public:
    StartDescriptionState(ParseContext& parseContext)
        : parseContext(parseContext) {
    }

    void process(char symbol) override {
        if (symbol == '#') {
            parseContext.addToken(Scat::TokenId::Description);

            parseContext.changeState(ParseState::Command);
        } else if (symbol != ' ' && symbol != '\t') {
            parseContext.addSymbol(symbol);

            parseContext.changeState(ParseState::Description);
        }
    }

    void addToken(std::function<void(std::string&)> preprocess) override {
        parseContext.addToken(Scat::TokenId::Description, preprocess);
    }
private:
    ParseContext& parseContext;
};

class DescriptionState : public AbstractParseState {
public:
    DescriptionState(ParseContext& parseContext)
        : parseContext(parseContext) {
    }

    void process(char symbol) override {
        if (symbol == '\n') {
            parseContext.addToken(Scat::TokenId::Description, rightTrimToken);

            parseContext.changeState(ParseState::Init);
        } else if (symbol == '#') {
            parseContext.addToken(Scat::TokenId::Description);

            parseContext.changeState(ParseState::Command);
        } else {
            parseContext.addSymbol(symbol);
        }
    }

    void addToken(std::function<void(std::string&)> preprocess) override {
        parseContext.addToken(Scat::TokenId::Description, preprocess);
    }
private:
    ParseContext& parseContext;
};

class CommandState : public AbstractParseState {
public:
    CommandState(ParseContext& parseContext)
        : parseContext(parseContext) {
    }

    void process(char symbol) override {
        if (std::isalnum(symbol)) {
            parseContext.addSymbol(symbol);
        } else if (symbol == '(') {
            parseContext.addSymbol(symbol);

            parseContext.changeState(ParseState::CommandArguments);
        } else {
            parseContext.changeState(ParseState::Error);
        }
    }
private:
    ParseContext& parseContext;
};

class CommandArgumentsState : public AbstractParseState {
public:
    CommandArgumentsState(ParseContext& parseContext)
        : parseContext(parseContext) {
    }

    void process(char symbol) override {
        if (symbol == '\n') {
            parseContext.changeState(ParseState::Error);
        } else if (symbol == ')') {
            parseContext.addSymbol(symbol);

            parseContext.addToken(Scat::TokenId::Command);
            parseContext.changeState(ParseState::Description);
        } else {
            parseContext.addSymbol(symbol);
        }
    }

    void addToken(std::function<void(std::string&)> preprocess) override {
        parseContext.addToken(Scat::TokenId::Command, preprocess);
    }
private:
    ParseContext& parseContext;
};

class ErrorState : public AbstractParseState {
public:
    ErrorState(ParseContext& parseContext)
        : parseContext(parseContext) {
    }

    void process(char symbol) override {
        throw std::runtime_error("Cannot parse scenario");
    }
private:
    ParseContext& parseContext;
};

using CreateStateFunction = std::function<std::shared_ptr<AbstractParseState>(ParseContext&)>;

std::unordered_map<ParseState, CreateStateFunction> createStateFunctions = {
    { ParseState::Init,             createStateFunction<InitState> },
    { ParseState::Id,               createStateFunction<IdState> },
    { ParseState::IdSpace,          createStateFunction<IdSpaceState> },
    { ParseState::StartDescription, createStateFunction<StartDescriptionState> },
    { ParseState::Description,      createStateFunction<DescriptionState> },
    { ParseState::Command,          createStateFunction<CommandState> },
    { ParseState::CommandArguments, createStateFunction<CommandArgumentsState> },
    { ParseState::Error,            createStateFunction<ErrorState> },
};

ParseContext::ParseContext() {
    for (const auto& [currentState, makeState] : createStateFunctions) {
        states[currentState] = makeState(*this);
    }

    state = states.at(ParseState::Init);
}

void ParseContext::addSymbol(char currentSymbol) {
    this->tokenName.push_back(currentSymbol);
}

void ParseContext::changeState(ParseState newState) {
    auto it = this->states.find(newState);
    if (it != std::cend(this->states)) {
        state = it->second;
    } else {
        throw std::runtime_error("Invalid parse state");
    }
}

void ParseContext::addTokenRightTrim() {
    getState()->addToken(rightTrimToken);
}

void ParseContext::addToken(Scat::TokenId tokenId, std::function<void(std::string&)> preprocess) {
    if (!this->tokenName.empty()) {
        preprocess(this->tokenName);

        this->tokens.push_back(
            Scat::Token(tokenId, this->tokenName)
        );

        clearToken();
    }
}

void ParseContext::clearToken() {
    this->tokenName.clear();
}

ParseContext::ParseStateHolder ParseContext::getState() {
    return this->state;
}

Scat::Tokens ParseContext::extractTokens() {
    return std::move(this->tokens);
}

void ParseContext::handleStreamSymbol(char currentSymbol) {
    getState()->process(currentSymbol);
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

