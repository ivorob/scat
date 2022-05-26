#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

namespace {

void usage(const std::string& application) {
    std::cerr << "usage:" << std::endl
              << "\t" << fs::path(application).filename().native() << " testfile"
              << std::endl;
}

}

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
