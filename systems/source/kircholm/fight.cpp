#include "kircholm/kirch_fight.h"

#include "toc/core/log.h"

namespace kirch {

FightSystem::FightSystem(const std::shared_ptr<Scenario>& scenario,
                         SystemState* system) noexcept
    : _scenario{scenario}, _system{system} {
    app_assert(_system != nullptr, "Invalid system");
}

struct DirectFightData {
    std::array<std::set<Unit::IdType>, 2> ids{};
};

std::set<HexCoordinate> FightSystem::get_control_zone_of(
    const std::set<Unit::IdType>& units) const {
    std::set<HexCoordinate> res;
    _scenario->units.apply_for_each<PositionComponent>([&units, &res](const auto& cmp) {
        if (units.find(cmp.owner()) != units.cend() & cmp.position.has_value()) {
            const auto pos = *cmp.position;
            res.insert(pos);
            const auto neighbors = pos.neighbors();
            res.insert(neighbors.cbegin(), neighbors.cend());
        }
        return true;
    });
    return res;
}

std::set<Unit::IdType> FightSystem::find_units_in_zone(
    const std::set<Unit::IdType>& units, const std::set<HexCoordinate>& zone) const {
    std::set<Unit::IdType> res;
    for (const auto& unit : units) {
        const auto pc = _scenario->units.get_component<PositionComponent>(unit);
        if (pc == nullptr)
            continue;
        if (!pc->position)
            continue;
        if (zone.count(*pc->position) == 1)
            res.insert(unit);
    }
    return res;
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

std::vector<DirectFightResult> FightSystem::process_fights() const {
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

    const auto [d_positions, d_zone] = get_positions(defenders);
    const auto [a_positions, a_zone] = get_positions(attackers);

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

    while (d_positions.empty() || a_positions.empty()) {
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
        data.ids[0] = d_zone.at(it->second);
        data.ids[1].insert(it->first);
    }

    for (auto it = attackers.begin(); it != attackers.end(); it = attackers.begin()) {
        const auto unit = *it;
        attackers.erase(it);
        const auto d_zone_it = d_zone.find(a_pos_it->second);
        if (d_zone_it == d_zone.cend())
            continue;
        auto& data  = data_vec.emplace_back();
        data.ids[0] = d_zone_it->second;
        defenders.erase(data.ids[0].cbegin(), data.ids[0].cend());

        data.ids[1].insert(unit);
        bool stop = false;
        while (!stop) {
            std::set<HexCoordinate> d_z;
            for (const auto& d_u : data.ids[0]) {
                const auto it = d_positions.find(d_u);
                if (it == d_positions.cend()) {
                }
            }
        }
    }
}

}  // namespace kirch