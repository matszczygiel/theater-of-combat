#include "systems.h"

#include "toc/core/log.h"

#include "kircholm.h"

namespace tsys {
std::unique_ptr<System> create_system(const std::string_view name) {
    constexpr auto systems_list = list_available_systems();

    const auto it = std::find(std::cbegin(systems_list), std::cend(systems_list), name);
    if (it == std::cend(systems_list)) {
        app_assert_throw(true, "Unknown system name: {}!", name.data());
        return nullptr;
    }
    const auto pos = std::distance(std::cbegin(systems_list), it);

    switch (pos) {
        case 0:
            return std::make_unique<kirch::SystemKircholm>();

        default:
            break;
    }
    return nullptr;
}

}  // namespace tsys