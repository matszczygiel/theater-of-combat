#include "unit.h"

#include "core/log.h"
#include "unit_components.h"

Unit::IdType UnitManager::create(UnitType type, std::string name,
                                 bool assign_components) {
    auto id = _current_free_id++;
    app_assert(_units.count(id) == 0,
               "Error in UnitManager, creating unit with currently used id.");
    auto& unit = _units[id];
    unit.name  = name;
    unit.type  = type;

    if (assign_components)
        assign_default_components(id, type);
}

void UnitManager::assign_default_components(Unit::IdType id, UnitType type) {
    switch (type) {
        case UnitType::heavy:
            assign_component<MovementComponent>(id, type, 12);
            break;
        case UnitType::mechanized:
            assign_component<MovementComponent>(id, type, 20);
            break;
        default:
            app_assert(false, "Unknown unit type.");
            break;
    }
}