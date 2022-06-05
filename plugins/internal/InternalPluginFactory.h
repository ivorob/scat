#pragma once

#include "Plugin.h"

namespace Scat {

class InternalPluginFactory {
public:
    InternalPluginFactory() = default;

    PluginHolder create(const std::string& name) const;
};

}
