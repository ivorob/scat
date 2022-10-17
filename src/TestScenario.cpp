#include "TestScenario.h"

Scat::TestScenario::TestScenario(
    std::string description,
    std::string given,
    std::string when,
    std::string then
) : description(std::move(description))
  , given(std::move(given))
  , when(std::move(when))
  , then(std::move(then)) {
}

const std::string& Scat::TestScenario::getDescription() const {
    return this->description;
}

const std::string& Scat::TestScenario::getGiven() const {
    return this->given;
}

const std::string& Scat::TestScenario::getWhen() const {
    return this->when;
}

const std::string& Scat::TestScenario::getThen() const {
    return this->then;
}
