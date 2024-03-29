#pragma once

#include <string>

namespace Scat {

enum class TokenId { 
    None,
    Id, 
    Description,
    Command
};

class Token {
public:
    Token(TokenId id, std::string name);

    TokenId getId() const;
    const std::string& getName() const;

    bool operator==(const Token& other) const;
private:
    const TokenId id;
    const std::string name;
};

}  // namespace Scat
