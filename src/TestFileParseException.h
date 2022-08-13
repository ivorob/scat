#pragma once

#include <exception>
#include <string>

namespace Scat {

class TestFileParseException : public std::exception {
public:
    TestFileParseException(
        size_t lineNumber,
        size_t symbolIndex, 
        std::string text 
    );

    size_t getLineNumber() const;
    size_t getSymbolIndex() const;
    const std::string& getText() const;
    
    const char* what() const noexcept override;
private:
    const size_t lineNumber = 0;
    const size_t symbolIndex = 0;
    const std::string text;
    std::string exceptionDescription;
};

}
