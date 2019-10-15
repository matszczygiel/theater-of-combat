#include "unit/lua_units.h"

#include <sol/sol.hpp>

#include "core/lua_vm.h"
#include "unit/unit.h"
#include "unit/unit_components.h"
#include "unit/unit_manager.h"

namespace units {
static void lua_push_components(sol::state& lua, sol::usertype<UnitManager>& manager) {
    auto movement =
        lua.new_usertype<MovementComponent>("MovementComponent", sol::no_constructor);

    movement["total_moving_pts"] = &MovementComponent::total_moving_pts;
    movement["moving_pts"]       = &MovementComponent::moving_pts;
    movement["position"]         = &MovementComponent::position;

    manager["assign_movement_cmp"] =
        &UnitManager::assign_component<MovementComponent, int>;

    manager["remove_movement_cmp"] = &UnitManager::remove_component<MovementComponent>;
}

void lua_push_functions(sol::state& lua) {
    lua.new_enum("UnitType", "heavy", UnitType::heavy, "mechanized",
                 UnitType::mechanized);

    auto unit_man =
        lua.new_usertype<UnitManager>("UnitManager", sol::constructors<UnitManager>());

    unit_man["create"] = &UnitManager::create;

    lua_push_components(lua, unit_man);
}
}  // namespace units
