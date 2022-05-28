#pragma once

#include <string>
#include <vector>

namespace Scat {

class TestCommand {
public:
    using Arguments = std::vector<std::string>;
public:
    TestCommand(std::string name, Arguments arguments = {});

    const std::string& getName() const;
    const Arguments& getArguments() const;
private:
    const std::string name;
    const Arguments arguments;
};

}
