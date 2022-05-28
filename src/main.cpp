#include <iostream>
#include <fstream>

#include "TestFileParser.h"

namespace {

void usage() {
    std::cerr << "usage:" << std::endl
              << "\t" << "scat testfile"
              << std::endl;
}

std::ostream& operator<<(std::ostream& output, const std::vector<std::string>& arguments) {
    for (const auto& argument : arguments) {
        output << argument << " ";
    }

    return output;
}

}

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        usage();
        return EXIT_FAILURE;
    }

    std::ifstream input(argv[1]);
    if (input.good()) {
        Scat::TestFileParser testFileParser(input);

        while (auto testCommand = testFileParser.parseNext()) {
            std::cout << "Execute test command: " << testCommand->getName() 
                      << std::endl
                      << "\twith arguments: " << testCommand->getArguments()
                      << std::endl;
        }
    } else {
        std::cerr << "File isn't found: " << argv[1] << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
