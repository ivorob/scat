#pragma once

#include <istream>
#include <optional>

#include "TestScenario.h"

namespace Scat {

class TestFileParser {
public:
    TestFileParser(std::istream& input);

    std::optional<TestScenario> parseNext();
private:
    std::istream& input;
};

}
