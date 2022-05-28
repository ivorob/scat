#include "TestCommand.h"

Scat::TestCommand::TestCommand(std::string name, std::vector<std::string> arguments)
    : name(std::move(name))
    , arguments(std::move(arguments))
{
}

const std::string&
Scat::TestCommand::getName() const
{
    return this->name;
}

const Scat::TestCommand::Arguments&
Scat::TestCommand::getArguments() const
{
    return this->arguments;
}
