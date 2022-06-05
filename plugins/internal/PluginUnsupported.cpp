#include <iostream>

#include "PluginUnsupported.h"

Scat::PluginUnsupported::PluginUnsupported()
    : Plugin("Unsupported")
{
}

bool
Scat::PluginUnsupported::execute(const std::vector<std::string>&)
{
    std::cout << "\tCOMMAND IS UNSUPPORTED!" << std::endl;
    return false;
}
