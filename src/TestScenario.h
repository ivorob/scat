#pragma once

#include <iostream>

namespace Scat {

class TestScenario {
public:
    TestScenario(
        std::string description,
        std::string given,
        std::string when,
        std::string then
    );

    const std::string& getDescription() const;
    const std::string& getGiven() const;
    const std::string& getWhen() const;
    const std::string& getThen() const;
private:
    const std::string description;
    const std::string given;
    const std::string when;
    const std::string then;
};

}
