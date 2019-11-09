#include "kircholm/kirch_mover.h"

#include "toc/core/log.h"

namespace kirch {
/*
static WeightedBidirectionalGraph<Map::SiteId, Movability> make_weighted_graph(
    const Map& map, UnitType type) {
    const auto river_tab = get_river_table(type);
    const auto hex_tab   = get_hex_table(type);

    WeightedBidirectionalGraph<Map::SiteId, Movability> res(map.graph(), 0);

    for (const auto& [node, neighbors] : map.graph().adjacency_matrix()) {
        const auto site_type = map.type_of(node);

        switch (site_type) {
            case Map::SiteType::hex: {
                const auto hex_type = map.hexes().at(node).type();

                for (const auto& neighbor : neighbors) {
                    const auto w = res.weight(neighbor, node);
                    res.change_edge_weight(neighbor, node, w + hex_tab.at(hex_type));
                }
            } break;
            case Map::SiteType::river: {
                const auto riv_type = map.rivers().at(node).type();
                engine_assert(neighbors.size() == 2, "River should have 2 neighbors.");
                const auto hex1 = *neighbors.begin();
                const auto hex2 = *(++neighbors.begin());

                const auto w12 = res.weight(hex1, node) + res.weight(node, hex2) +
                                 river_tab.at(riv_type);

                const auto w21 = res.weight(hex2, node) + res.weight(node, hex1) +
                                 river_tab.at(riv_type);

                res.remove_node(node);
                res.insert_edge(hex1, hex2, w12, w21);
            } break;

            default:
                engine_assert(false, "Unknown site type in computing weighted graph.");
                break;
        }
    }
    return res;
}
*/
MovementSystem::MovementSystem(const std::shared_ptr<Scenario>& scenario) noexcept
    : _scenario{scenario} {}

bool MovementSystem::init_movement(HexCoordinate coord, std::vector<std::string> teams,
                                   std::vector<std::string> hostile_teams) {
    engine_assert_throw(!is_moving(), "Already moving unit.");
    engine_info("Initiating MovementSystem.");
    /*
        std::set<Unit::IdType> friendly;
        for (const auto& t : teams) {
            if (auto it = _scenario->teams.find(t); it != _scenario->teams.end()) {
                auto set = it->second;
                friendly.merge(set);
            }
        }

        std::set<Unit::IdType> hostile;
        for (const auto& t : hostile_teams) {
            if (auto it = _scenario->teams.find(t); it != _scenario->teams.end()) {
                auto set = it->second;
                hostile.merge(set);
            }
        }

        _scenario->units.apply_for_each<MovementComponent>(
            [this, &coord, &friendly](auto& cmp) {
                if (cmp.position == coord && friendly.count(cmp.owner()) == 1) {
                    if (!cmp.immobilized) {
                        _target_component = std::addressof(cmp);
                        return false;
                    }
                }
                return true;
            });

        if (!is_moving()) {
            reset();
            return false;
        }

        _scenario->units.apply_for_each<MovementComponent>([this, &hostile](auto& cmp) {
            if (hostile.count(cmp.owner()) == 1 && cmp.position) {
                const auto id = _scenario->map.get_hex_id(cmp.position.value()).value();
                _sticky_sites.merge(_scenario->map.get_controlable_hexes_from(id));
            }
            return true;
        });

        _sticky_sites.erase(_scenario->map.get_hex_id(coord).value());

        const auto graph =
            make_weighted_graph(_scenario->map, _target_component->owner_type());

        const auto start_hex = _scenario->map.get_hex_id(coord);
        engine_assert_throw(start_hex.has_value(),
                            "Initializing movement with nonexistent hex on this map.");

        std::tie(_distances, _paths) = graph.dijkstra(*start_hex);
        for (auto i = _distances.begin(); i != _distances.end();) {
            if (i->second > _target_component->moving_pts) {
                engine_assert(
                    _paths.erase(i->first) == 1,
                    "Paths does not containing such node, error in dijkstra algorithm ?");
                i = _distances.erase(i);
            } else {
                ++i;
            }
        }*/
    return true;
}

bool MovementSystem::is_moving() const noexcept {
    app_assert(bool(_target_mc) == bool(_target_pc),
               "Target components are not in the same state.");
    return _target_pc != nullptr;
}

void MovementSystem::reset() noexcept {
    _target_mc = nullptr;
    _target_pc = nullptr;
    _distances.clear();
    _paths.clear();
    _sticky_sites.clear();
}

std::vector<std::unique_ptr<Action>> MovementSystem::move_target(
    HexCoordinate destination) {
    const auto path = path_indices(destination);

    if (path.empty()) {
        reset();
        return {};
    }
/*
    const auto find = std::find_first_of(path.begin(), path.end(), _sticky_sites.begin(),
                                         _sticky_sites.end());

    const bool immobilized  = find != path.end();
    const auto true_dest_id = immobilized ? *find : path.back();

    const auto cost = _distances.at(true_dest_id);
    auto new_cmp    = *_target_component;
    new_cmp.moving_pts -= cost;
    engine_assert(new_cmp.moving_pts >= 0, "Unit {} has negative number of moving pts.",
                  new_cmp.owner());
    new_cmp.position    = _scenario->map.get_hex_coord(true_dest_id);
    new_cmp.immobilized = immobilized;
    reset();
    return std::make_unique<ComponentChangeAction<MovementComponent>>(std::move(new_cmp));*/
}

std::vector<Map::SiteId> MovementSystem::path_indices(HexCoordinate destination) const {
    engine_assert_throw(is_moving(), "No unit to preview path.");
    const auto dest_hex = _scenario->map.get_hex_id(destination);
    if (!dest_hex)
        return {};

    auto it = _distances.find(dest_hex.value());
    if (it == _distances.end())
        return {};

    std::vector<Map::SiteId> res = {*dest_hex};
    auto i                       = _paths.find(dest_hex.value());
    while (i != _paths.end()) {
        res.push_back(i->second);
        i = _paths.find(res.back());
    }

    std::reverse(res.begin(), res.end());

    return res;
}

std::vector<HexCoordinate> MovementSystem::path_preview(HexCoordinate destination) const {
    const auto ids = path_indices(destination);
    std::vector<HexCoordinate> res;
    res.reserve(ids.size());
    for (const auto& id : ids) {
        res.push_back(_scenario->map.get_hex_coord(id).value());
    }

    return res;
}

}  // namespace kirch