#include <iostream>
#include <fstream>

#include "TestFileParser.h"

namespace {

void usage() {
    std::cerr << "usage:" << std::endl
              << "\t" << "scat testfile"
              << std::endl;
}

std::ostream& operator<<(std::ostream& output, const Scat::TestScenario& scenario) {
    std::cout << "[   Scenario ]\t" << scenario.getDescription() << std::endl
              << "[------------]\t----------------------------------" << std::endl
              << "[      Given ]\t" << scenario.getGiven() << std::endl
              << "[       When ]\t" << scenario.getWhen() << std::endl
              << "[       Then ]\t" << scenario.getThen() << std::endl
              << "[ RUN        ]\t" << std::endl
              << "[         OK ]\t" << std::endl
              << "[------------]" << std::endl << std::endl;

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
        while (auto testScenario = testFileParser.parseNext()) {
            std::cout << *testScenario << std::endl;
        }
    } else {
        std::cerr << "File isn't found: " << argv[1] << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
