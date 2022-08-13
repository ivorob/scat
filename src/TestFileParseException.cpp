#include <sstream>

#include "TestFileParseException.h"

Scat::TestFileParseException::TestFileParseException(
    size_t lineNumber,
    size_t symbolIndex, 
    std::string text 
)   : lineNumber(lineNumber)
    , symbolIndex(symbolIndex)
    , text(std::move(text))
{
    std::ostringstream description;
    description << "Parse exception on line " << lineNumber
                << " in " << symbolIndex << " position. "
                << "Expectation: Scenario block. Text: \""
                << this->text << "\".";
    exceptionDescription = description.str();
}

size_t Scat::TestFileParseException::getLineNumber() const
{
    return this->lineNumber;
}

size_t Scat::TestFileParseException::getSymbolIndex() const
{
    return this->symbolIndex;
}

const std::string& Scat::TestFileParseException::getText() const
{
    return this->text;
}

const char* Scat::TestFileParseException::what() const noexcept
{
    return this->exceptionDescription.c_str();
}
