#include "component.h"
#include <string>

std::map<std::string, std::function<std::shared_ptr<Component>(void)>>& Component::get_register() {
    static std::map<std::string, std::function<std::shared_ptr<Component>(void)>> reg;
    return reg;
}

std::shared_ptr<Component> Component::create(const std::string& name) {
    auto& reg = get_register();
    auto it = reg.find(name);

    if (it == reg.end()) {
        // This happens when there is no component registered to this
        // name. Here I return a null pointer, but you can handle this
        // error differently if it suits you better.
        return nullptr;
    }

    return it->second();
}
