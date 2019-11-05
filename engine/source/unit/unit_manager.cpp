#include "unit/unit_manager.h"

#include <tuple>

void ComponentPoll::remove_components(Unit::IdType id) {
    for (auto& [type, vec] : components) {
        vec->remove_component(id);
    }
}

Unit::IdType UnitManager::create(Unit::KindType type, const std::string& name) {
    const auto id = _id_gen.fetch();
    engine_assert(_units.count(id) == 0,
                  "Error in UnitManager, creating unit with currently used id {}.", id);
    auto [unit_it, success] = _units.insert({id, Unit()});
    engine_assert(success, "");

    unit_it->second._name = name;
    unit_it->second._type = type;

    assign_component<PositionComponent>(id);
    return id;
}

void UnitManager::remove(Unit::IdType id) {
    engine_assert_throw(_units.count(id) == 1, "Removing nonexistent unit with id {}",
                        id);
    engine_assert(_units.erase(id) == 1, "");
    _id_gen.return_to_poll(id);
    _components.remove_components(id);
}

UnitManager UnitManager::create_test_manager() {
    UnitManager um{};
    const auto u0 = um.create(0, "test unit 0");
    const auto u1 = um.create(0, "test unit 1");
    const auto u2 = um.create(1, "test unit 2");

    um.get_component<PositionComponent>(u0)->position = HexCoordinate(-1, 1);
    um.get_component<PositionComponent>(u1)->position = HexCoordinate(-3, 0);
    um.get_component<PositionComponent>(u2)->position = HexCoordinate(-2, 2);
    return um;
}

const std::map<Unit::IdType, Unit>& UnitManager::units() const noexcept { return _units; }