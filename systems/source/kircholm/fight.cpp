#include "kircholm/kirch_fight.h"

#include <numeric>

#include "toc/core/log.h"
#include "toc/core/randomize.h"

#include "kircholm/kirch_actions.h"
#include "kircholm/kirch_components.h"
#include "kircholm/kirch_system.h"

namespace kirch {

DirectFightSystem::DirectFightSystem(SystemKircholm* system) noexcept
    : ComponentSystem{system} {}

std::pair<std::map<Unit::IdType, HexCoordinate>,
          std::map<HexCoordinate, std::set<Unit::IdType>>>
DirectFightSystem::get_positions(const std::set<Unit::IdType>& unit_set) const {
    std::map<Unit::IdType, HexCoordinate> res0;
    std::map<HexCoordinate, std::set<Unit::IdType>> res1;

    units().apply_for_each<PositionComponent>([&unit_set, &res0, &res1](const auto& cmp) {
        if (unit_set.find(cmp.owner()) != unit_set.cend() && cmp.position.has_value()) {
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

std::vector<DirectFightData> DirectFightSystem::generate_data_vec() const {
    auto attackers = get_player_units(_sys->current_player_index());
    auto defenders = get_player_units(_sys->opposite_player_index());

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
            data.deffender_units.merge(d_to_insert);

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
            data.attacker_units.merge(a_to_insert);

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

struct FightTableEntry {
    int realtive_probability{10};
    Strength att_loss{0};
    Strength def_loss{0};
    // positive value means attackers win
    Movability break_through{0};
    double disorganization_abs_prob{0.0};
};
struct FightTableIndex {
    Strength def_min_strength{};
    Strength def_max_strength{};
    // negative value means that there are more deffenders
    int modifier{};
};

using Entry = std::pair<FightTableIndex, std::vector<FightTableEntry>>;
// clang-format off
const static std::array<Entry, 20> fight_table = {
    Entry{{1, 3, -2}, {{25, 3, 0, -3, 0.5 }, {25, 2, 0, -2, 0.25}, {25, 1, 0, -1, 0.0 }, {25, 1, 1,  0, 0.0 }}},
    Entry{{1, 3, -1}, {{25, 3, 0, -3, 0.5 }, {25, 2, 0, -2, 0.25}, {25, 1, 1, -1, 0.0 }, {25, 1, 1,  0, 0.0 }}},
    Entry{{1, 3,  0}, {{25, 3, 0, -2, 0.25}, {25, 2, 0, -2, 0.0 }, {25, 1, 1, -1, 0.0 }, {25, 1, 2,  1, 0.0 }}},
    Entry{{1, 3,  1}, {{25, 3, 0, -2, 0.25}, {25, 2, 0, -1, 0.0 }, {25, 1, 1,  0, 0.0 }, {25, 1, 2,  1, 0.0 }}},
    Entry{{1, 3,  2}, {{25, 2, 0, -2, 0.1 }, {25, 1, 0,  0, 0.0 }, {25, 1, 1,  1, 0.0 }, {25, 0, 2,  1, 0.0 }}},
    Entry{{1, 3,  3}, {{25, 1, 0, -1, 0.1 }, {25, 1, 1,  0, 0.0 }, {25, 1, 1,  1, 0.0 }, {25, 0, 2,  2, 0.1 }}},
    Entry{{1, 3,  4}, {{25, 1, 0, -1, 0.1 }, {25, 1, 2,  0, 0.0 }, {25, 1, 3,  1, 0.1 }, {25, 0, 2,  2, 0.25}}},
    Entry{{1, 3,  5}, {{25, 1, 1, -1, 0.0 }, {25, 1, 2,  1, 0.0 }, {25, 1, 3,  2, 0.1 }, {25, 0, 3,  3, 0.25}}},
    Entry{{1, 3,  6}, {{25, 1, 1, -1, 0.0 }, {25, 1, 2,  2, 0.0 }, {25, 1, 3,  3, 0.25}, {25, 0, 3,  3, 0.5 }}},
    Entry{{1, 3,  7}, {{25, 1, 2,  0, 0.0 }, {25, 1, 2,  2, 0.1 }, {25, 0, 3,  3, 0.5 }, {25, 0, 3,  3, 0.75}}},
    
    Entry{{4, 6, -2}, {{25, 4, 0, -3, 0.5 }, {25, 3, 0, -2, 0.25}, {25, 2, 0, -1, 0.0 }, {25, 2, 1,  0, 0.0 }}},
    Entry{{4, 6, -1}, {{25, 4, 0, -3, 0.5 }, {25, 3, 0, -2, 0.25}, {25, 2, 1, -1, 0.0 }, {25, 2, 2,  0, 0.0 }}},
    Entry{{4, 6,  0}, {{25, 4, 0, -2, 0.25}, {25, 3, 0, -2, 0.0 }, {25, 2, 1, -1, 0.0 }, {25, 2, 2,  1, 0.0 }}},
    Entry{{4, 6,  1}, {{25, 4, 0, -2, 0.25}, {25, 3, 1, -1, 0.0 }, {25, 2, 2,  0, 0.0 }, {25, 2, 3,  1, 0.0 }}},
    Entry{{4, 6,  2}, {{25, 4, 0, -2, 0.1 }, {25, 2, 1,  0, 0.0 }, {25, 2, 2,  1, 0.0 }, {25, 1, 3,  1, 0.0 }}},
    Entry{{4, 6,  3}, {{25, 2, 0, -1, 0.1 }, {25, 2, 2,  0, 0.0 }, {25, 2, 2,  1, 0.0 }, {25, 0, 3,  2, 0.1 }}},
    Entry{{4, 6,  4}, {{25, 2, 0, -1, 0.1 }, {25, 2, 2,  0, 0.0 }, {25, 2, 3,  1, 0.1 }, {25, 0, 3,  2, 0.25}}},
    Entry{{4, 6,  5}, {{25, 2, 2, -1, 0.0 }, {25, 2, 3,  1, 0.0 }, {25, 1, 4,  2, 0.1 }, {25, 0, 4,  3, 0.25}}},
    Entry{{4, 6,  6}, {{25, 2, 2, -1, 0.0 }, {25, 1, 3,  2, 0.0 }, {25, 1, 4,  3, 0.25}, {25, 0, 4,  3, 0.5 }}},
    Entry{{4, 6,  7}, {{25, 2, 3,  0, 0.0 }, {25, 1, 3,  2, 0.1 }, {25, 0, 4,  3, 0.5 }, {25, 0, 4,  3, 0.75}}}
    };
// clang-format on
const static int lowest_mod   = -2;
const static int highest_mod  = 7;
const static int max_strength = 6;

static int get_modifier_from_strength(const Strength& attackers,
                                      const Strength& deffenders) {
    if (attackers >= deffenders)
        return attackers / deffenders;
    else
        return deffenders / attackers - 2;
}

Strength DirectFightSystem::accumulate_strength(
    const std::set<Unit::IdType>& units_set) const {
    Strength res = 0;
    for (const auto& id : units_set) {
        const auto cmp = units().get_component<DirectFightComponent>(id);
        if (cmp == nullptr)
            continue;
        res += cmp->strength_pts;
    }
    return res;
}

DirectFightResult DirectFightSystem::process_fight(const DirectFightData& data) const {
    const auto attackers = accumulate_strength(data.attacker_units);
    const auto defenders = accumulate_strength(data.deffender_units);

    const auto mod = std::clamp(get_modifier_from_strength(attackers, defenders),
                                lowest_mod, highest_mod);

    const auto ref_strength = std::min(defenders, max_strength);

    const auto it = std::find_if(fight_table.cbegin(), fight_table.cend(),
                                 [&ref_strength, &mod](const auto& element) {
                                     if (element.first.modifier != mod)
                                         return false;
                                     if (element.first.def_min_strength <= ref_strength &&
                                         element.first.def_max_strength >= ref_strength) {
                                         return true;
                                     }
                                     return false;
                                 });
    app_assert(it != fight_table.cend(),
               "Result of fight must be present in table. Ref str: {}, mod: {}",
               ref_strength, mod);
    const auto& entry_vec = it->second;

    std::vector<int> probs{};
    for (const auto& entry : entry_vec) {
        auto input = entry.realtive_probability;
        if (!probs.empty())
            input += probs.back();
        probs.push_back(input);
    }
    app_assert(probs.size() == entry_vec.size(), "");
    std::uniform_int_distribution dis{1, probs.back()};
    const auto rand = dis(randomize::engine());
    int i           = 1;
    for (; i < static_cast<int>(probs.size()); ++i) {
        if (probs[i] >= rand) {
            break;
        }
    }
    app_assert(--i < static_cast<int>(probs.size()), "");
    auto res_entry = entry_vec[i];
    DirectFightResult res;
    res.break_through = res_entry.break_through;

    // defenders
    if (res_entry.def_loss >= defenders) {
        res.units_destroyed.insert(data.deffender_units.cbegin(),
                                   data.deffender_units.cend());
    } else {
        std::vector<std::tuple<Unit::IdType, Strength, Strength>> def;
        for (const auto& u : data.deffender_units) {
            const auto cmp = units().get_component<DirectFightComponent>(u);
            app_assert(cmp != nullptr,
                       "Unit involved in direct fight should have apropiate component.");
            def.emplace_back(std::make_tuple(u, cmp->strength_pts, 0));
        }

        std::uniform_int_distribution unit_dis{0, static_cast<int>(def.size() - 1)};
        while (res_entry.def_loss-- > 0) {
            const auto rand = unit_dis(randomize::engine());
            auto& [u, s, l] = def[rand];
            app_assert(s != 0, "Strength should be strictly positive.");
            ++l;
            if (l == s) {
                res.units_destroyed.insert(u);
                def.erase(def.begin() + rand);
                unit_dis =
                    std::uniform_int_distribution{0, static_cast<int>(def.size() - 1)};
            }
        }

        for (const auto& [u, s, l] : def) {
            if (l != 0)
                res.losses[u] = l;
        }
    }
    // attackers
    if (res_entry.att_loss >= attackers) {
        res.units_destroyed.insert(data.attacker_units.cbegin(),
                                   data.attacker_units.cend());
    } else {
        std::vector<std::tuple<Unit::IdType, Strength, Strength>> att;
        for (const auto& u : data.attacker_units) {
            const auto cmp = units().get_component<DirectFightComponent>(u);
            app_assert(cmp != nullptr,
                       "Unit involved in direct fight should have apropiate component.");
            att.emplace_back(std::make_tuple(u, cmp->strength_pts, 0));
        }

        std::uniform_int_distribution unit_dis{0, static_cast<int>(att.size() - 1)};
        while (res_entry.att_loss-- > 0) {
            const auto rand = unit_dis(randomize::engine());
            auto& [u, s, l] = att[rand];
            app_assert(s != 0, "Strength should be strictly positive.");
            ++l;
            if (l == s) {
                res.units_destroyed.insert(u);
                att.erase(att.begin() + rand);
                unit_dis =
                    std::uniform_int_distribution{0, static_cast<int>(att.size() - 1)};
            }
        }

        for (const auto& [u, s, l] : att) {
            if (l != 0)
                res.losses[u] = l;
        }
    }

    std::uniform_real_distribution disorganisation_dist;
    res.disorganisation =
        disorganisation_dist(randomize::engine()) < res_entry.disorganization_abs_prob;
    res.ids = data;
    return res;
}

void DirectFightSystem::init_direct_fights() {
    const auto data = generate_data_vec();
    if (data.empty())
        return;

    std::vector<DirectFightResult> results{data.size()};
    std::generate(results.begin(), results.end(), [i = 0, this, &data]() mutable {
        const auto res = process_fight(data[i]);
        app_debug("Fight result [{}]", i++);
        app_debug("    Attacker units:");
        for (const auto& u : res.ids.attacker_units)
            app_debug("        {}", u);
        app_debug("    Deffender units:");
        for (const auto& u : res.ids.deffender_units)
            app_debug("        {}", u);
        app_debug("    Units destroyed:");
        for (const auto& u : res.units_destroyed)
            app_debug("        {}", u);
        app_debug("    Losses:");
        for (const auto& [u, s] : res.losses)
            app_debug("        unit {} lost {} ", u, s);
        app_debug("    Break through: {}", res.break_through);
        app_debug("    Disorganization: {}", res.disorganisation);

        app_debug("Fight result ");
        return res;
    });
    for (const auto& res : results)
        send_actions(res);

    push_action<DirectFightResultComputed>(results);
}

void DirectFightSystem::send_actions(const DirectFightResult& res) {
    for (const auto& u : res.units_destroyed)
        push_action<UnitDestroyedAction>(u);
    for (const auto& [u, l] : res.losses) {
        auto cmp_old = units().get_component<DirectFightComponent>(u);
        app_assert(cmp_old != nullptr, "");
        DirectFightComponent cmp = *cmp_old;
        cmp.strength_pts -= l;
        push_action<ComponentChangeAction<DirectFightComponent>>(cmp);
    }
    if (res.break_through > 0) {
        for (const auto& u : res.ids.deffender_units) {
            auto cmp = units().get_component<MovementComponent>(u);
            app_assert(cmp != nullptr, "");
            cmp->moving_pts = res.break_through;
        }
    } else if (res.break_through < 0) {
        for (const auto& u : res.ids.attacker_units) {
            auto cmp = units().get_component<MovementComponent>(u);
            app_assert(cmp != nullptr, "");
            cmp->moving_pts = -res.break_through * 10;
        }
    }
}

}  // namespace kirch