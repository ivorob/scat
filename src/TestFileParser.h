#pragma once

#include <istream>
#include <optional>

#include "TestCommand.h"

namespace Scat {

class TestFileParser {
public:
    TestFileParser(std::istream& input);

    std::optional<TestCommand> parseNext();
private:
    std::istream& input;
};

}
