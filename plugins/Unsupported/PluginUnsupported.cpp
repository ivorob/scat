#include "PluginUnsupported.h"

Scat::PluginUnsupported::PluginUnsupported()
    : Plugin("Unsupported")
{
}

bool
Scat::PluginUnsupported::execute(const std::vector<std::string>& arguments)
{
    return false;
}
