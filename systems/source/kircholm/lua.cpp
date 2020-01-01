#include "kircholm/kirch_lua.h"

#include <sol/sol.hpp>

#include "toc/unit/unit_manager.h"

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
    auto unit_type = lua.new_enum("UnitType", UnitType::infrantry, "infrantry",
                                  UnitType::artillery, "artillery", UnitType::cavalary,
                                  "cavalary", UnitType::dragoons, "dragoons");
    auto fighting_performance =
        lua.new_enum("FightingPerformance", FightingPerformance::level_1, "level_1",
                     FightingPerformance::level_2, "level_2");

    auto movement = lua.new_usertype<MovementComponent>(
        "MovementComponent",
        sol::constructors<MovementComponent(), MovementComponent(Movability)>());

    movement["moving_pts"]  = &MovementComponent::moving_pts;
    movement["immobilized"] = &MovementComponent::immobilized;
    movement["formation"]   = &MovementComponent::formation;

    if (lua["UnitManager"] != sol::lua_nil) {
        lua["UnitManager"]["assign_movement_cmp"] =
            &UnitManager::assign_component<MovementComponent, MovementComponent&>;
        lua["UnitManager"]["remove_movement_cmp"] =
            &UnitManager::remove_component<MovementComponent>;
    }

    auto direct_fight = lua.new_usertype<DirectFightComponent>(
        "DirectFightComponent",
        sol::constructors<DirectFightComponent(), DirectFightComponent(Strength)>());

    direct_fight["strength_pts"] = &DirectFightComponent::strength_pts;
    direct_fight["in_fight"]     = &DirectFightComponent::in_fight;

    if (lua["UnitManager"] != sol::lua_nil) {
        lua["UnitManager"]["assign_direct_fight_cmp"] =
            &UnitManager::assign_component<DirectFightComponent, DirectFightComponent&>;
        lua["UnitManager"]["remove_direct_fight_cmp"] =
            &UnitManager::remove_component<DirectFightComponent>;
    }
}
}  // namespace kirch
