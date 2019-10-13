#include "lua_units.h"

#include <sol/sol.hpp>

#include "toc/core/lua_vm.h"
#include "toc/unit/unit.h"
#include "toc/unit/unit_manager.h"
#include "toc/unit/unit_components.h"

namespace units {
static void lua_push_components(sol::usertype<UnitManager>& manager) {
    auto& lua = lua::get_state();

    auto movement = lua.new_usertype<MovementComponent>("MovementComponent",
                                                        sol::no_constructor);

    movement["total_moving_pts"] =
        &MovementComponent::total_moving_pts;
    movement["moving_pts"] = &MovementComponent::moving_pts;
    movement["position"]   = &MovementComponent::position;

    manager["assign_movement_cmp"] =
        &UnitManager::assign_component<MovementComponent, int>;

    manager["remove_movement_cmp"] =
        &UnitManager::remove_component<MovementComponent>;
}

void lua_push_functions() {
    auto& lua = lua::get_state();

    lua.new_enum("UnitType", "heavy", UnitType::heavy, "mechanized",
                 UnitType::mechanized);

    auto unit_man = lua.new_usertype<UnitManager>(
        "UnitManager", sol::constructors<UnitManager>());

    unit_man["create"] = &UnitManager::create;

    lua_push_components(unit_man);
}
}  // namespace units
