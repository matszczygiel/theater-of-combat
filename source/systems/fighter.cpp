#include "fighter.h"

FightingSystem::FightingSystem(const GameState& state) : _scenario{state.scenario} {
    app_assert(_scenario != nullptr, "Scenario is not initialized.");
}

void FightingSystem::make_fight_stack(const int attacking_player_index) {
    app_assert(_fight_stack.empty(), "_fight_stack must be cleared first.");

    std::array<std::set<Unit::IdType>, 2> all_units;
    for (int pl_index = 0; pl_index < all_units.size(); ++pl_index) {
        for (const auto& t : _scenario->player_teams[pl_index]) {
            if (auto it = _scenario->teams.find(t); it != _scenario->teams.end()) {
                auto set = it->second;
                all_units[pl_index].merge(set);
            }
        }
    }

    const int deffending_player_index = (attacking_player_index + 1) % 2;

    auto& um        = _scenario->units;
    const auto& map = _scenario->map;

    auto get_opponent_units_in_control_zone = [&](Unit::IdType id) -> decltype(auto) {
        std::set<Unit::IdType> res;

        for (int i = 0; i < all_units.size(); ++i)
            if (all_units.at(i).count(id) == 1)
                if (const auto mc = um.get_component<MovementComponent>(id); mc) {
                    if (mc->position) {
                        const auto controlable = map.get_controlable_hexes_from(
                            map.get_hex_id(mc->position.value()).value());

                        for (const auto& unit : all_units.at((i + 1) % 2)) {
                            const auto u_mc = um.get_component<MovementComponent>(unit);
                            if (!u_mc)
                                continue;
                            if (!u_mc->position)
                                continue;

                            const auto pos_id =
                                map.get_hex_id(u_mc->position.value()).value();

                            if (controlable.count(pos_id) == 1)
                                res.insert(unit);
                        }
                    }
                }
        return res;
    };

    std::set<Unit::IdType> involved_units{};

    for (const auto& att_unit : all_units.at(attacking_player_index)) {
        if (involved_units.count(att_unit) == 1)
            continue;

        auto defenders = get_opponent_units_in_control_zone(att_unit);
        if (defenders.empty())
            continue;

        FightData fd;
        fd.ids.at(attacking_player_index).insert(att_unit);
        fd.ids.at(deffending_player_index).insert(defenders.begin(), defenders.end());
        involved_units.merge(defenders);
        involved_units.insert(att_unit);

        for (int pl_id = deffending_player_index;; pl_id = (pl_id + 1) % 2) {
            for (const auto& u_id : fd.ids.at(pl_id)) {
                if (involved_units.count(u_id) == 1)
                    continue;
                auto opponents = get_opponent_units_in_control_zone(u_id);
                if (opponents.empty())
                    continue;

                involved_units.insert(opponents.begin(), opponents.end());
                fd.ids.at((pl_id + 1) % 2).merge(opponents);
            }
        }

        _fight_stack.push_back(fd);
    }
}