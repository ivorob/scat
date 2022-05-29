#include "Plugin.h"

Scat::Plugin::Plugin(std::string name)
    : name(std::move(name))
{
}

const std::string&
Scat::Plugin::getName() const
{
    return this->name;
}
