#include "kircholm/kirch_components.h"
#include "kircholm/kirch_fight.h"

#include "toc/core/log.h"

namespace kirch {

FightSystem::FightSystem(const std::shared_ptr<Scenario>& scenario,
                         SystemState* system) noexcept
    : _scenario{scenario}, _system{system} {
    app_assert(_system != nullptr, "Invalid system");
}

std::pair<std::map<Unit::IdType, HexCoordinate>,
          std::map<HexCoordinate, std::set<Unit::IdType>>>
FightSystem::get_positions(const std::set<Unit::IdType>& units) const {
    std::map<Unit::IdType, HexCoordinate> res0;
    std::map<HexCoordinate, std::set<Unit::IdType>> res1;
    _scenario->units.apply_for_each<PositionComponent>(
        [&units, &res0, &res1](const auto& cmp) {
            if (units.find(cmp.owner()) != units.cend() && cmp.position.has_value()) {
                const auto pos    = *cmp.position;
                res0[cmp.owner()] = pos;
                res1[pos].insert(cmp.owner());
                const auto neighbors = pos.neighbors();
                for (const auto& n : neighbors)
                    res1[n].insert(cmp.owner());
            }
            return true;
        });
    return {res0, res1};
}

static std::set<HexCoordinate> get_control_zone_of(
    const std::set<Unit::IdType>& units,
    const std::map<Unit::IdType, HexCoordinate>& positions) {
    std::set<HexCoordinate> res;
    for (const auto& u : units) {
        const auto it = positions.find(u);
        if (it == positions.cend())
            continue;
        const auto& pos = it->second;
        res.insert(pos);
        const auto neighbors = pos.neighbors();
        for (const auto& n : neighbors)
            res.insert(n);
    }
    return res;
}

std::vector<DirectFightData> FightSystem::generate_data_vec() const {
    const auto& attacker_teams = _scenario->player_teams[_system->current_player_index()];
    std::set<Unit::IdType> attackers;
    for (const auto& t : attacker_teams) {
        if (auto it = _scenario->teams.find(t); it != _scenario->teams.end()) {
            auto set = it->second;
            attackers.merge(set);
        }
    }

    const auto& defender_teams =
        _scenario->player_teams[_system->opposite_player_index()];
    std::set<Unit::IdType> defenders;
    for (const auto& t : defender_teams) {
        if (auto it = _scenario->teams.find(t); it != _scenario->teams.end()) {
            auto set = it->second;
            defenders.merge(set);
        }
    }

    auto [d_positions, d_zone] = get_positions(defenders);
    auto [a_positions, a_zone] = get_positions(attackers);

    for (auto it = attackers.begin(); it != attackers.end();) {
        if (a_positions.find(*it) == a_positions.cend())
            it = attackers.erase(it);
        else
            ++it;
    }

    for (auto it = defenders.begin(); it != defenders.end();) {
        if (d_positions.find(*it) == d_positions.cend())
            it = defenders.erase(it);
        else
            ++it;
    }

    // Create instances of fights
    std::vector<DirectFightData> data_vec{};

    while (!(d_positions.empty() || a_positions.empty())) {
        auto it = a_positions.begin();
        for (; it != a_positions.end();) {
            if (d_zone.find(it->second) == d_zone.cend())
                it = a_positions.erase(it);
            else
                break;
        }
        if (it == a_positions.end())
            break;
        auto& data = data_vec.emplace_back();

        std::set<Unit::IdType> d_to_insert = d_zone.at(it->second);
        std::set<Unit::IdType> a_to_insert = {it->first};
        while (true) {
            const auto d_test_zone = get_control_zone_of(d_to_insert, d_positions);
            for (const auto& d_i : d_to_insert)
                d_positions.erase(d_i);
            data.ids[0].merge(d_to_insert);

            for (const auto& d_test : d_test_zone) {
                const auto it = a_zone.find(d_test);
                if (it == a_zone.cend())
                    continue;
                a_to_insert.insert(it->second.cbegin(), it->second.cend());
            }
            if (a_to_insert.empty())
                break;

            const auto a_test_zone = get_control_zone_of(a_to_insert, a_positions);
            for (const auto& a_i : a_to_insert)
                a_positions.erase(a_i);
            data.ids[1].merge(a_to_insert);

            for (const auto& a_test : a_test_zone) {
                const auto it = d_zone.find(a_test);
                if (it == d_zone.cend())
                    continue;
                d_to_insert.insert(it->second.cbegin(), it->second.cend());
            }
            if (d_to_insert.empty())
                break;
        }
    }
    return data_vec;
}
enum class FightSide { attack, defence, none };
struct FightTableEntry {
    int realtive_probability{10};
    Strength att_loss{0};
    Strength def_loss{0};
    // positive value means attackers win
    Movability retreat{0};
    double disorganization_abs_prob{0.0};
};
struct FightTableIndex {
    Strength def_max_strength;
    // negative value means that there are more deffenders
    int ratio;
};

const static std::map<FightTableIndex, std::vector<FightTableEntry>> fight_table = {
    {{3, -4},
     {{25, 3, 0, -3, 0.5},
      {25, 2, 0, -2, 0.25},
      {25, 1, 0, -1, 0.0},
      {25, 1, 1, 0, 0.0}}},
    {{6, -4},
     {{25, 4, 0, -3, 0.5},
      {25, 3, 0, -2, 0.25},
      {25, 2, 0, -1, 0.0},
      {25, 2, 1, 0, 0.0}}}};

DirectFightResult FightSystem::process_fight(const DirectFightData& data) const {
    

}

std::vector<DirectFightResult> FightSystem::process_fights() const {
    const auto data = generate_data_vec();
}

}  // namespace kirch