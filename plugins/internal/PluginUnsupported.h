#pragma once

#include "Plugin.h"

namespace Scat {

class PluginUnsupported : public Plugin {
public:
    PluginUnsupported();

    bool execute(const std::vector<std::string>& arguments) override;
};

}
