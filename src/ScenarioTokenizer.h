#pragma once

#include <istream>
#include <list>
#include <string>

#include "Token.h"

namespace Scat {

using Tokens = std::list<Token>;

class ScenarioTokenizer {
public:
    ScenarioTokenizer(std::istream& input);

    Tokens tokenize();
private:

private:
    std::istream& input;
};

}  // namespace Scat
