#include "unit.h"

#include "core/log.h"
#include "unit_components.h"

const std::string& Unit::name() const { return _name; }

UnitType Unit::type() { return _type; }

Unit::IdType ComponentBase::owner() { return _owner; }

UnitType ComponentBase::owner_type() { return _owner_type; }

Unit::IdType UnitManager::create(UnitType type, std::string name,
                                 bool assign_components) {
    auto id = _current_free_id++;
    app_assert(_units.count(id) == 0,
               "Error in UnitManager, creating unit with currently used id.");
    auto& unit = _units[id];
    unit._name  = name;
    unit._type  = type;

    if (assign_components)
        assign_default_components(id, type);

    return id;
}

void UnitManager::assign_default_components(Unit::IdType id, UnitType type) {
    switch (type) {
        case UnitType::heavy:
            assign_component<MovementComponent>(id, 12);
            break;
        case UnitType::mechanized:
            assign_component<MovementComponent>(id, 20);
            break;
        default:
            app_assert(false, "Unknown unit type.");
            break;
    }
}

const std::map<Unit::IdType, Unit>& UnitManager::units() const {
    return _units;
}


// list all used components for cereal

#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/polymorphic.hpp>

CEREAL_REGISTER_TYPE(ComponentVec<MovementComponent>);
CEREAL_REGISTER_POLYMORPHIC_RELATION(ComponentVecBase, ComponentVec<MovementComponent>);


