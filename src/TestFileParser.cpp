#include "TestFileParser.h"

namespace {

std::string_view skipSpaces(std::string_view line)
{
    while (!line.empty() && std::isspace(line[0])) {
        line.remove_prefix(1);
    }

    return line;
}

std::string_view extractWord(std::string_view& line)
{
    std::string_view word = line;

    size_t count = 0;
    size_t size = line.size();
    while (!line.empty() && !std::isspace(line[0])) {
        line.remove_prefix(1);
        ++count;
    }

    word.remove_suffix(size - count);
    return word;
}

std::vector<std::string_view> splitWords(std::string_view line)
{
    std::vector<std::string_view> words;
    while (!line.empty()) {
        line = skipSpaces(line);
        auto word = extractWord(line);
        if (!word.empty()) {
            words.push_back(word);
        }
        line = skipSpaces(line);
    }

    return words;
}

}

Scat::TestFileParser::TestFileParser(std::istream& input)
    : input(input)
{
}

std::optional<Scat::TestCommand>
Scat::TestFileParser::parseNext()
{
    std::string line;
    while (std::getline(input, line)) {
        if (line.empty()) {
            continue;
        }

        auto words = splitWords(line);
        if (words.size() == 1) {
            return TestCommand(std::string(words[0]));
        } else if (words.size() > 1) {
            const std::string name { words[0] };
            std::vector<std::string> arguments { words.begin() + 1, words.end() };
            return TestCommand(name, arguments);
        }
    }

    return std::nullopt;
}
