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

WeightedUnidirectionalGraph<std::pair<Map::SiteId, int>, Movability>
MovementSystem::make_weighted_graph(Unit::IdType id) const {
    const auto hex_tab = get_movability_table(_scenario->units, id);
    const auto rot_tab = get_rotation_table();

    const auto& map = _scenario->map;

    WeightedUnidirectionalGraph<std::pair<Map::SiteId, int>, Movability> res(map.graph(),
                                                                             0);

    for (const auto& [node, neighbors] : map.graph().adjacency_matrix()) {
        const auto& [node_id, dir] = node;
        const auto site_type       = map.type_of(node_id);

        switch (site_type) {
            case Map::SiteType::hex: {
                const auto hex_type =
                    static_cast<HexType>(map.hexes().at(node_id).type());

                for (const auto& neighbor : neighbors) {
                    if (neighbor.first == node_id) {
                        res.change_edge_weight(neighbor, node, rot_tab.at(hex_type));
                    } else {
                        app_assert(
                            neighbor.second == dir,
                            "in kirch system, direction cannot be changed by non-rotation.");
                        const auto op_dir = HexCoordinate::opposite_direction(dir);
                        res.change_edge_weight({neighbor.first, op_dir},
                                               {node_id, op_dir}, hex_tab.at(hex_type));
                    }
                }
            } break;
            default:
                app_critical("Kircholm only supports hex sites on map.");
                app_assert(false, "Unknown site type in computing weighted graph.");
                break;
        }
    }
    return res;
}

MovementSystem::MovementSystem(const std::shared_ptr<Scenario>& scenario,
                               SystemState* system) noexcept
    : _scenario{scenario}, _system{system} {
    app_assert(_system != nullptr, "Invalid system");
}

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
    if (_target_mc->moving_pts == 0) {
        reset();
        return false;
    }

    _scenario->units.apply_for_each<PositionComponent>([this, &hostile](auto& cmp) {
        if (hostile.count(cmp.owner()) == 1 && cmp.position) {
            const auto id = _scenario->map.get_hex_id(cmp.position.value()).value();
            const auto controlable = _scenario->map.get_controlable_hexes_from(id);
            for (const auto& c : controlable) {
                _sticky_sites.push_back(_scenario->map.get_hex_coord(c).value());
            }
        }
        return true;
    });

    _searcher.reset(make_weighted_graph(_target_pc->owner()));

    const auto start_hex = _scenario->map.get_hex_id(coord);
    engine_assert_throw(start_hex.has_value(),
                        "Initializing movement with nonexistent hex on this map.");

    _searcher.search_path(*start_hex, _target_pc->direction.value(),
                          _target_mc->moving_pts);
    return true;
}

bool MovementSystem::is_moving() const noexcept {
    app_assert(bool(_target_mc) == bool(_target_pc),
               "Target components are not in the same state.");
    return _target_pc != nullptr;
}

void MovementSystem::reset() noexcept {
    app_debug("Restting MovementSystem");
    _target_mc   = nullptr;
    _target_pc   = nullptr;
    _immobilized = false;
    _sticky_sites.clear();
    _path.clear();
}

std::vector<std::tuple<HexCoordinate, int, Movability>> MovementSystem::path_preview(
    HexCoordinate destination) const {
    app_assert_throw(is_moving(), "No unit to preview path.");
    const auto dest_hex = _scenario->map.get_hex_id(destination);
    if (!dest_hex)
        return {};

    const auto ids = _searcher.path_indices(*dest_hex, 0);
    std::vector<std::tuple<HexCoordinate, int, Movability>> res;
    res.reserve(ids.size());
    int prev_id;
    for (const auto& [id, dir, mov] : ids) {
        if (res.size() != 0 && prev_id == id)
            continue;
        res.push_back({_scenario->map.get_hex_coord(id).value(), dir, mov});
        prev_id = id;
    }
    return res;
}

bool MovementSystem::is_hex_set() const noexcept { return !_path.empty() && is_moving(); }

bool MovementSystem::set_target_hex(HexCoordinate hex) {
    _path = path_preview(hex);

    if (_path.empty()) {
        reset();
        return false;
    }

    const auto find = std::find_first_of(
        _path.begin(), _path.end(), _sticky_sites.begin(), _sticky_sites.end(),
        [](const auto& p, const auto& s) { return std::get<0>(p) == s; });

    _immobilized = find != _path.end();
    if (_immobilized) {
        _path.erase(find, _path.end());
        return move();
    }
    return true;
}

std::map<int, Movability> MovementSystem::get_avaliable_dirs() {
    app_assert(is_hex_set(), "Hex must be set.");
    const auto& [site, dir, cost] = _path.back();
    const auto site_id            = _scenario->map.get_hex_id(site).value();
    _searcher.search_rotation(site_id, dir, _target_mc->moving_pts - cost);
    std::map<int, Movability> res;
    for (const auto& [dir, d_cost] : _searcher.distances())
        res[dir.second] = d_cost;
    return res;
}

bool MovementSystem::set_target_dir(int dir) {
    const auto t_dir = dir % HexCoordinate::neighbors_count();
    const auto dirs  = get_avaliable_dirs();
    const auto it    = dirs.find(t_dir);
    if (it == dirs.end()) {
        reset();
        return false;
    }
    _path.push_back({std::get<0>(_path.back()), it->first, it->second});
    return move();
}

bool MovementSystem::move() {
    app_debug("Moving through path:");
    for (const auto& [hex, dir, cost] : _path) {
        app_debug("hex: ({}, {}), dir: {}, cost: {}", hex.q(), hex.p(), dir, cost);
    }
    app_debug(" ");

    const auto& [td_coord, td_dir, td_mov] = _path.back();

    auto new_mc = *_target_mc;
    new_mc.moving_pts -= td_mov;
    engine_assert(new_mc.moving_pts >= 0, "Unit {} has negative number of moving pts.",
                  new_mc.owner());
    new_mc.immobilized = _immobilized;
    _system->push_action(
        std::make_unique<ComponentChangeAction<MovementComponent>>(std::move(new_mc)));

    auto new_pc      = *_target_pc;
    new_pc.position  = td_coord;
    new_pc.direction = td_dir;
    _system->push_action(
        std::make_unique<ComponentChangeAction<PositionComponent>>(std::move(new_pc)));

    reset();
    return true;
}

const std::vector<std::tuple<HexCoordinate, int, Movability>>& MovementSystem::path()
    const {
    return _path;
}

void PathSearcher::reset(
    const WeightedUnidirectionalGraph<std::pair<Map::SiteId, int>, Movability>& graph) {
    _graph = graph;
}

void PathSearcher::search_path(Map::SiteId start, int start_dir, Movability max_cost) {
    std::tie(_distances, _paths) = _graph.dijkstra({start, start_dir});
    for (auto i = _distances.begin(); i != _distances.end();) {
        if (i->second > max_cost) {
            app_assert(
                _paths.erase(i->first) == 1,
                "Paths does not containing such node, error in dijkstra algorithm ?");
            i = _distances.erase(i);
        } else {
            ++i;
        }
    }
}

void PathSearcher::search_rotation(Map::SiteId site, int start_dir, Movability max_cost) {
    WeightedUnidirectionalGraph<std::pair<Map::SiteId, int>, Movability> graph;
    for (const auto& [idx, neighbors] : _graph.adjacency_matrix()) {
        if (idx.first == site) {
            graph.insert_node(idx);
            for (const auto& [n_id, w] : neighbors) {
                if (n_id.first == site)
                    graph.insert_edge(idx, n_id, w);
            }
        }
    }

    std::tie(_distances, _paths) = graph.dijkstra({site, start_dir});
    for (auto i = _distances.begin(); i != _distances.end();) {
        if (i->second > max_cost) {
            app_assert(
                _paths.erase(i->first) == 1,
                "Paths does not containing such node, error in dijkstra algorithm ?");
            i = _distances.erase(i);
        } else {
            ++i;
        }
    }
}

const std::map<std::pair<Map::SiteId, int>, Movability>& PathSearcher::distances() const {
    return _distances;
}

const std::map<std::pair<Map::SiteId, int>, std::pair<Map::SiteId, int>>&
PathSearcher::paths() const {
    return _paths;
}

std::vector<std::tuple<Map::SiteId, int, Movability>> PathSearcher::path_indices(
    Map::SiteId destination, int direction) const {
    auto it = _distances.find({destination, direction});
    if (it == _distances.end())
        return {};

    std::vector<std::tuple<Map::SiteId, int, Movability>> res = {
        {destination, direction, it->second}};

    auto i = _paths.find({destination, direction});
    while (i != _paths.end()) {
        const auto& [site, dir] = i->second;
        res.push_back({site, dir, _distances.at(i->second)});
        i = _paths.find(i->second);
    }

    std::reverse(res.begin(), res.end());
    return res;
}

}  // namespace kirch