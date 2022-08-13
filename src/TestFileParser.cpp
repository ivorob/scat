#include "TestFileParser.h"

Scat::TestFileParser::TestFileParser(std::istream& input)
    : input(input)
{
}

std::optional<Scat::TestScenario> Scat::TestFileParser::parseNext()
{
    return std::nullopt;
}
