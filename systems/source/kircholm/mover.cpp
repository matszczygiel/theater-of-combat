#include "kircholm/kirch_mover.h"
#include "kircholm/kirch_types.h"

#include "toc/core/log.h"

namespace kirch {

// If no such type in map, then it is inaccessible
static std::map<HexType, Movability> get_movability_table(const UnitManager& um,
                                                          Unit::IdType id) {
    const auto type = static_cast<UnitType>(um.units().at(id).type());
    switch (type) {
        case UnitType::infrantry:
            return {
                {HexType::field, 1},      {HexType::forest, 2},
                {HexType::hills, 2},      {HexType::hillside_0, 2},
                {HexType::hillside_1, 2}, {HexType::hillside_2, 2},
                {HexType::hillside_3, 2}, {HexType::hillside_4, 2},
                {HexType::hillside_5, 2}, {HexType::swamp, 3},
            };
        case UnitType::artillery:
            return {
                {HexType::field, 1},      {HexType::forest, 2},
                {HexType::hills, 2},      {HexType::hillside_0, 2},
                {HexType::hillside_1, 2}, {HexType::hillside_2, 2},
                {HexType::hillside_3, 2}, {HexType::hillside_4, 2},
                {HexType::hillside_5, 2},
            };
        case UnitType::cavalary:
            return {
                {HexType::field, 1},      {HexType::forest, 2},
                {HexType::hills, 2},      {HexType::hillside_0, 2},
                {HexType::hillside_1, 2}, {HexType::hillside_2, 2},
                {HexType::hillside_3, 2}, {HexType::hillside_4, 2},
                {HexType::hillside_5, 2},
            };
        case UnitType::dragoons: {
            std::map<HexType, Movability> res = {
                {HexType::field, 1},      {HexType::forest, 2},
                {HexType::hills, 2},      {HexType::hillside_0, 2},
                {HexType::hillside_1, 2}, {HexType::hillside_2, 2},
                {HexType::hillside_3, 2}, {HexType::hillside_4, 2},
                {HexType::hillside_5, 2},
            };

            const auto dc = um.get_component<DragoonComponent>(id);
            if (dc)
                if (dc->on_foot) {
                    res[HexType::swamp] = 3;
                }
            return res;
        }
    }
    app_assert(false, "Unknown UnitType.");
}

static std::map<HexType, Movability> get_rotation_table() {
    return {
        {HexType::field, 1},      {HexType::forest, 1},     {HexType::hills, 1},
        {HexType::hillside_0, 1}, {HexType::hillside_1, 1}, {HexType::hillside_2, 1},
        {HexType::hillside_3, 1}, {HexType::hillside_4, 1}, {HexType::hillside_5, 1},
        {HexType::swamp, 1},
    };
}

static WeightedBidirectionalGraph<std::pair<Map::SiteId, int>, Movability>
make_weighted_graph(const Map& map, const UnitManager& um, Unit::IdType id) {
    const auto hex_tab = get_movability_table(um, id);
    const auto rot_tab = get_rotation_table();

    WeightedBidirectionalGraph<std::pair<Map::SiteId, int>, Movability> res(map.graph(),
                                                                            0);

    for (const auto& [node, neighbors] : map.graph().adjacency_matrix()) {
        const auto& [node_id, dir] = node;
        const auto site_type       = map.type_of(node_id);

        switch (site_type) {
            case Map::SiteType::hex: {
                const auto hex_type =
                    static_cast<HexType>(map.hexes().at(node_id).type());

                for (const auto& neighbor : neighbors) {
                    const auto w = res.weight(neighbor, node);
                    if (neighbor.first == node_id) {
                        res.change_edge_weight(neighbor, node, w + rot_tab.at(hex_type));
                    } else {
                        res.change_edge_weight(neighbor, node, w + hex_tab.at(hex_type));
                    }
                }
            } break;
            case Map::SiteType::border: {
                engine_assert(neighbors.size() == 2, "River should have 2 neighbors.");
                const auto site0 = *neighbors.begin();
                const auto site1 = *(++neighbors.begin());

                const auto w01 = res.weight(site0, node) + res.weight(node, site1);
                const auto w10 = res.weight(site1, node) + res.weight(node, site0);

                res.remove_node(node);
                res.insert_edge(site0, site1, w01, w10);
            } break;

            default:
                engine_assert(false, "Unknown site type in computing weighted graph.");
                break;
        }
    }
    return res;
}

MovementSystem::MovementSystem(const std::shared_ptr<Scenario>& scenario) noexcept
    : _scenario{scenario} {}

bool MovementSystem::init_movement(HexCoordinate coord, std::vector<std::string> teams,
                                   std::vector<std::string> hostile_teams) {
    engine_assert_throw(!is_moving(), "Already moving unit.");
    engine_info("Initiating MovementSystem.");

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

    _scenario->units.apply_for_each<PositionComponent>(
        [this, &coord, &friendly](auto& cmp) {
            if (cmp.position == coord && friendly.count(cmp.owner()) == 1) {
                _target_pc = std::addressof(cmp);
                return false;
            }
            return true;
        });

    if (!_target_pc) {
        reset();
        return false;
    }

    _target_mc = _scenario->units.get_component<MovementComponent>(_target_pc->owner());
    if (!_target_mc) {
        reset();
        return false;
    }

    _scenario->units.apply_for_each<PositionComponent>([this, &hostile](auto& cmp) {
        if (hostile.count(cmp.owner()) == 1 && cmp.position) {
            const auto id = _scenario->map.get_hex_id(cmp.position.value()).value();
            _sticky_sites.merge(_scenario->map.get_controlable_hexes_from(id));
        }
        return true;
    });

    _sticky_sites.erase(_scenario->map.get_hex_id(coord).value());

    const auto graph =
        make_weighted_graph(_scenario->map, _scenario->units, _target_pc->owner());

    const auto start_hex = _scenario->map.get_hex_id(coord);
    engine_assert_throw(start_hex.has_value(),
                        "Initializing movement with nonexistent hex on this map.");

    std::tie(_distances, _paths) =
        graph.dijkstra({*start_hex, _target_pc->direction.value()});
    for (auto i = _distances.begin(); i != _distances.end();) {
        if (i->second > _target_mc->moving_pts) {
            engine_assert(
                _paths.erase(i->first) == 1,
                "Paths does not containing such node, error in dijkstra algorithm ?");
            i = _distances.erase(i);
        } else {
            ++i;
        }
    }
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
    HexCoordinate destination, int direction) {
    const auto path = path_indices(destination, direction);

    if (path.empty()) {
        reset();
        return {};
    }

    const auto find = std::find_first_of(
        path.begin(), path.end(), _sticky_sites.begin(), _sticky_sites.end(),
        [](const auto& p, const auto& s) { return p.first == s; });

    const bool immobilized  = find != path.end();
    const auto true_dest_id = immobilized ? *find : path.back();

    std::vector<std::unique_ptr<Action>> res;
    const auto cost = _distances.at(true_dest_id);
    auto new_mc     = *_target_mc;
    new_mc.moving_pts -= cost;
    engine_assert(new_mc.moving_pts >= 0, "Unit {} has negative number of moving pts.",
                  new_mc.owner());
    new_mc.immobilized = immobilized;
    res.push_back(
        std::make_unique<ComponentChangeAction<MovementComponent>>(std::move(new_mc)));

    auto new_pc      = *_target_pc;
    new_pc.position  = _scenario->map.get_hex_coord(true_dest_id.first);
    new_pc.direction = true_dest_id.second;
    res.push_back(
        std::make_unique<ComponentChangeAction<PositionComponent>>(std::move(new_pc)));

    reset();
    return res;
}

std::vector<std::pair<Map::SiteId, int>> MovementSystem::path_indices(
    HexCoordinate destination, int direction) const {
    engine_assert_throw(is_moving(), "No unit to preview path.");
    const auto dest_hex = _scenario->map.get_hex_id(destination);
    if (!dest_hex)
        return {};

    const std::pair<Map::SiteId, int> dest = {dest_hex.value(), direction};

    auto it = _distances.find(dest);
    if (it == _distances.end())
        return {};

    std::vector<std::pair<Map::SiteId, int>> res = {dest};
    auto i                                       = _paths.find(dest);
    while (i != _paths.end()) {
        res.push_back(i->second);
        i = _paths.find(res.back());
    }

    std::reverse(res.begin(), res.end());
    return res;
}

std::vector<std::pair<HexCoordinate, int>> MovementSystem::path_preview(
    HexCoordinate destination, int direction) const {
    const auto ids = path_indices(destination, direction);
    std::vector<std::pair<HexCoordinate, int>> res;
    res.reserve(ids.size());
    for (const auto& id : ids) {
        res.push_back({_scenario->map.get_hex_coord(id.first).value(), id.second});
    }

    return res;
}

}  // namespace kirch