#include "Token.h"

Scat::Token::Token(TokenId id, std::string name)
    : id(id)
    , name(std::move(name)) {
}

Scat::TokenId Scat::Token::getId() const {
    return this->id;
}

const std::string& Scat::Token::getName() const {
    return this->name;
}

bool Scat::Token::operator==(const Token& other) const {
    return this->id == other.id &&
           this->name == other.name;
}
