#include <iostream>
#include <fstream>

#include "TestFileParser.h"
#include "InternalPluginFactory.h"

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
        Scat::InternalPluginFactory internalPluginFactory;

        std::cout << "[-----------]" << std::endl;
        Scat::TestFileParser testFileParser(input);
        while (auto testCommand = testFileParser.parseNext()) {
            std::cout << "[ RUN       ] " << testCommand->getName() 
                << " "
                << testCommand->getArguments() << std::endl;

            bool successExecuted = false;

            auto plugin = internalPluginFactory.create(testCommand->getName());
            if (plugin) {
                std::cout << "[ CONTEXT   ]";
                successExecuted = plugin->execute(testCommand->getArguments());
            }

            if (successExecuted) {
                std::cout << "[        OK ]" << std::endl;
            } else {
                std::cout << "[      FAIL ]" << std::endl;
            }

            std::cout << "[-----------]" << std::endl;
        }
    } else {
        std::cerr << "File isn't found: " << argv[1] << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
