#include "unit.h"

#include "unit_components.h"
#include "core/log.h"

Unit::Unit(entityx::EntityManager &es, UnitType unit_type, std::string unit_name)
    : type{unit_type}, name{unit_name} {
    _entity = es.create();
    create_from_type(type);
}

Unit::~Unit() { _entity.destroy(); }

void Unit::create_from_type(UnitType type) {
    switch (type)
    {
    case UnitType::heavy:
        _entity.assign<MovementComponent>(type, 12);
        break;
    case UnitType::mechanized:
        _entity.assign<MovementComponent>(type, 20);
        break;
    default:
        app_assert(false, "Unknown unit type.");
        break;
    }
}