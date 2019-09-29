#include "fighter.h"

FightingSystem::FightingSystem(const GameState& state)
    : _scenario{state.scenario} {
    app_assert(_scenario != nullptr, "Scenario is not initialized.");
}

void FightingSystem::make_fight_stack() {
    app_assert(_fight_stack.empty(), "_fight_stack must be cleared first.");

    std::array<std::set<Unit::IdType>, 2> all_units;
    for (int pl_index = 0; pl_index < all_units.size(); ++pl_index) {
        for (const auto& t : _scenario->player_teams[pl_index]) {
            if (auto it = _scenario->teams.find(t);
                it != _scenario->teams.end()) {
                auto set = it->second;
                all_units[pl_index].merge(set);
            }
        }
    }

    auto& um        = _scenario->units;
    const auto& map = _scenario->map;

    std::set<Unit::IdType> involved_units{};
    for (const auto& [id, unit] : um.units()) {
        if (!um.get_component<FightComponent>(id)->in_fight ||
            involved_units.count(id) == 1)
            continue;

        involved_units.insert(id);
        FightData fd;

        const auto& pos =
            um.get_component<MovementComponent>(id)->position.value();
        fd.area.insert(map.get_hex_id(pos).value());

        decltype(fd.area) to_insert{};
        auto controlable = 
    }
}