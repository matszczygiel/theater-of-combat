#ifndef SYSTEMS_H
#define SYSTEMS_H

#include <array>
#include <memory>
#include <string_view>

#include "toc/gameplay/system.h"

namespace tsys {

std::unique_ptr<System> create_system(const std::string_view name);

constexpr std::array<std::string_view, 1> list_available_systems() {
    return {"kircholm"};
}
}  // namespace tsys

#endif /* SYSTEMS_H */
