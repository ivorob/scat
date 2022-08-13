#include <iostream>
#include <fstream>

#include "TestFileParser.h"

namespace {

void usage() {
    std::cerr << "usage:" << std::endl
              << "\t" << "scat testfile"
              << std::endl;
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
        /*Scat::TestFileParser testFileParser(input);
        while (auto testScenario = testFileParser.parseNext()) {
            std::cout << testScenario << std::endl;
        }*/
    } else {
        std::cerr << "File isn't found: " << argv[1] << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
