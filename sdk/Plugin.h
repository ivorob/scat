#pragma once

#include <optional>
#include <string>
#include <vector>
#include <memory>

namespace Scat {

class Plugin {
public:
    Plugin(std::string name);
    virtual ~Plugin() = default;

    const std::string& getName() const;

    virtual bool execute(const std::vector<std::string>& arguments) = 0;
private:
    const std::string name;
};

using PluginHolder = std::unique_ptr<Plugin>;

}
