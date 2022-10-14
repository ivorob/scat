#pragma once

#include <istream>
#include <list>
#include <string>

#include "Token.h"

namespace Scat {

class ScenarioTokenizer {
public:
    ScenarioTokenizer(std::istream& input);

    std::list<Token> tokenize();
private:

private:
    std::istream& input;
};

}  // namespace Scat
