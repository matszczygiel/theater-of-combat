#include "unit_manager.h"

Unit::IdType UnitManager::create(UnitType type, const std::string& name,
                                 bool assign_components) {
    const auto id = _id_gen.fetch();
    engine_assert(_units.count(id) == 0,
                  "Error in UnitManager, creating unit with currently used id {}.", id);
    auto [unit_it, success] = _units.insert({id, Unit()});
    engine_assert(success, "");

    unit_it->second._name = name;
    unit_it->second._type = type;

    if (assign_components)
        assign_default_components(id, type);

    return id;
}

UnitManager UnitManager::create_test_manager() {
    UnitManager um{};
    const auto u0 = um.create(UnitType::mechanized, "test unit 0", true);
    const auto u1 = um.create(UnitType::mechanized, "test unit 1", true);
    const auto u2 = um.create(UnitType::heavy, "test unit 2", true);

    um.get_component<MovementComponent>(u0)->position = HexCoordinate(-1, 1);
    um.get_component<MovementComponent>(u1)->position = HexCoordinate(-3, 0);
    um.get_component<MovementComponent>(u2)->position = HexCoordinate(-2, 2);
    return um;
}

void UnitManager::assign_default_components(Unit::IdType id, UnitType type) {
    switch (type) {
        case UnitType::heavy:
            assign_component<MovementComponent>(id, 12);
            assign_component<FightComponent>(id, 6);
            break;
        case UnitType::mechanized:
            assign_component<MovementComponent>(id, 20);
            assign_component<FightComponent>(id, 4);
            break;
        default:
            engine_assert(false, "Unknown unit type {}.", static_cast<int>(type));
            break;
    }
}

const std::map<Unit::IdType, Unit>& UnitManager::units() const noexcept { return _units; }