#include "fighter.h"

#include <algorithm>
#include <iterator>
#include <numeric>

#include "toc/core/randomize.h"

FightingSystem::FightingSystem(const GameState& state) : _scenario{state.scenario} {
    app_assert(_scenario != nullptr, "Scenario is not initialized.");
}

void FightingSystem::make_fight_stack(const int attacking_player_index) {
    app_assert(_fight_stack.empty(), "_fight_stack must be cleared first.");

    std::array<std::set<Unit::IdType>, 2> all_units;
    for (int pl_index = 0; pl_index < static_cast<int>(all_units.size()); ++pl_index) {
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

        for (int i = 0; i < static_cast<int>(all_units.size()); ++i)
            if (all_units.at(i).count(id) == 1) {
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
                break;
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
        fd.attackers_index = attacking_player_index;
        fd.ids.at(attacking_player_index).insert(att_unit);
        fd.ids.at(deffending_player_index).merge(defenders);
        involved_units.insert(att_unit);

        for (int pl_id = deffending_player_index;; pl_id = (pl_id + 1) % 2) {
            std::set<Unit::IdType> to_insert{};
            for (const auto& u_id : fd.ids.at(pl_id)) {
                if (involved_units.count(u_id) == 1)
                    continue;
                to_insert.merge(get_opponent_units_in_control_zone(u_id));
            }
            if (to_insert.empty())
                break;

            involved_units.insert(fd.ids.at(pl_id).cbegin(), fd.ids.at(pl_id).cend());
            fd.ids.at((pl_id + 1) % 2).merge(to_insert);
        }

        for (const auto& player_units : fd.ids)
            for (const auto& unit : player_units) {
                const auto pos =
                    map.get_hex_id(
                           um.get_component<MovementComponent>(unit)->position.value())
                        .value();
                fd.area.insert(pos);
            }

        _fight_stack.push_back(fd);
    }
}

std::vector<std::unique_ptr<Action>> FightingSystem::compute_fight_result() {
    std::vector<std::unique_ptr<Action>> res;
    auto& um = _scenario->units;
    for (auto& fd : _fight_stack) {
        const int attackers_sum = std::accumulate(
            std::begin(fd.ids.at(fd.attackers_index)),
            std::end(fd.ids.at(fd.attackers_index)), 0, [&](int sum, int unit) {
                const auto fc = um.get_component<FightComponent>(unit);
                if (fc)
                    sum += fc->strength_pts;

                return sum;
            });

        const int defenders_sum = std::accumulate(
            std::begin(fd.ids.at((fd.attackers_index + 1) % 2)),
            std::end(fd.ids.at((fd.attackers_index + 1) % 2)), 0, [&](int sum, int unit) {
                const auto fc = um.get_component<FightComponent>(unit);
                if (fc)
                    sum += fc->strength_pts;

                return sum;
            });

        const int attackers_loss =
            std::uniform_int_distribution{0, defenders_sum / 3}(randomize::engine());
        const int defenders_loss =
            std::uniform_int_distribution{0, attackers_sum / 4}(randomize::engine());

        fd.retreat_distance = defenders_loss * 2 / (attackers_loss + 1);

        auto make_loses = [&](int loss, int index) {
            for (int i = 0; i < loss;) {
                int idx = std::uniform_int_distribution{
                    0, static_cast<int>(fd.ids.at(index).size())}(randomize::engine());

                auto it = fd.ids.at(index).begin();
                while (idx-- != 0)
                    std::next(it);
                const auto unit = *it;

                const auto fc = um.get_component<FightComponent>(unit);
                if (fc->strength_pts == 0)
                    continue;

                ++i;
                auto new_fc = *fc;
                auto new_mc = *um.get_component<MovementComponent>(unit);
                if (--new_fc.strength_pts == 0) {
                    new_mc.position = {};
                    fd.ids.at(index).erase(unit);
                }
                
                new_mc.immobilized = false;
                res.push_back(
                    std::make_unique<ComponentChangeAction<MovementComponent>>(new_mc));

                res.push_back(
                    std::make_unique<ComponentChangeAction<FightComponent>>(new_fc));
            }
        };

        make_loses(attackers_loss, fd.attackers_index);
        make_loses(defenders_loss, (fd.attackers_index + 1) % 2);
        fd.result_computed = true;
    }
    return res;
}

void FightingSystem::clear() { _fight_stack.clear(); }
