#include "unit/lua_units.h"

#include <sol/sol.hpp>

#include "unit/unit.h"
#include "unit/unit_components.h"
#include "unit/unit_manager.h"

namespace units {
void lua_push_functions(sol::state& lua) {
    auto position = lua.new_usertype<PositionComponent>(
        "PositionComponent",
        sol::constructors<PositionComponent(), PositionComponent(const HexCoordinate&),
                          PositionComponent(const HexCoordinate&, int)>());

    position["position"]  = &PositionComponent::position;
    position["direction"] = &PositionComponent::direction;

    auto unit_man =
        lua.new_usertype<UnitManager>("UnitManager", sol::constructors<UnitManager()>());

    unit_man["create"] = &UnitManager::create;

    unit_man["assign_position_cmp"] =
        &UnitManager::assign_component<PositionComponent, PositionComponent&>;

    unit_man["remove_position_cmp"] = &UnitManager::remove_component<PositionComponent>;
}
}  // namespace units
