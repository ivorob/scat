#pragma once

#include <string>

namespace Scat {

enum class TokenId { 
    Id, 
    Description
};

class Token {
public:
    Token(TokenId id, std::string name);

    TokenId getId() const;
    const std::string& getName() const;

private:
    const TokenId id;
    const std::string name;
};

}  // namespace Scat
