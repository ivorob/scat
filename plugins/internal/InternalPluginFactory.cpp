#include <unordered_map>
#include <functional>

#include "InternalPluginFactory.h"
#include "PluginUnsupported.h"

namespace {

template <typename T>
std::function<Scat::PluginHolder()> makeCreateFunction() {
    std::function<Scat::PluginHolder()> createFunction = []() -> Scat::PluginHolder {
        return std::make_unique<T>();
    };

    return createFunction;
}

const auto unsupported = makeCreateFunction<Scat::PluginUnsupported>();

std::unordered_map<std::string, std::function<Scat::PluginHolder()>> createFunctions =
{
};

}

Scat::PluginHolder
Scat::InternalPluginFactory::create(const std::string& name) const
{
    auto it = createFunctions.find(name);
    if (it != std::cend(createFunctions)) {
        return it->second();
    }

    return unsupported();
}
