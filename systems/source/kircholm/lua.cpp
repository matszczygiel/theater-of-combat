#include "kircholm/kirch_lua.h"

#include <sol/sol.hpp>

#include "kircholm/kirch_components.h"

namespace kirch {
void lua_push_functions(sol::state& lua) {
        auto movement =
        lua.new_usertype<MovementComponent>("MovementComponent", 
        sol::constructors<MovementComponent(), MovementComponent(Movability)>());

    movement["moving_pts"]  = &MovementComponent::moving_pts;
    movement["immobilized"] = &MovementComponent::immobilized;
    movement["formation"] = &MovementComponent::formation;
}
}  // namespace kirch
