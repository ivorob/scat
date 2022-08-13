#include "TestFileParser.h"
#include "TestFileParseException.h"

Scat::TestFileParser::TestFileParser(std::istream& input)
    : input(input)
{
}

std::optional<Scat::TestScenario> Scat::TestFileParser::parseNext()
{
    std::string line;
    if (std::getline(input, line)) {
        throw TestFileParseException(1, 0, "invalid scenario");
    }

    return std::nullopt;
}
