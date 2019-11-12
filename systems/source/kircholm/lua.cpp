#include "kircholm/kirch_lua.h"

#include <sol/sol.hpp>

#include "kircholm/kirch_components.h"
#include "kircholm/kirch_types.h"

namespace kirch {
void lua_push_functions(sol::state& lua) {
    auto hex_type =
        lua.new_enum("HexType", HexType::field, "field", HexType::forest, "forest",
                     HexType::swamp, "swamp", HexType::hillside_0, "hillside_0",
                     HexType::hillside_1, "hillside_1", HexType::hillside_2, "hillside_2",
                     HexType::hillside_3, "hillside_3", HexType::hillside_4, "hillside_4",
                     HexType::hillside_5, "hillside_5", HexType::hills, "hills");
    auto unit_type = lua.new_enum("", );

    auto movement = lua.new_usertype<MovementComponent>(
        "MovementComponent",
        sol::constructors<MovementComponent(), MovementComponent(Movability)>());

    movement["moving_pts"]  = &MovementComponent::moving_pts;
    movement["immobilized"] = &MovementComponent::immobilized;
    movement["formation"]   = &MovementComponent::formation;
}
}  // namespace kirch
