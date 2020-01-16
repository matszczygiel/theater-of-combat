#include "kircholm/kirch_mover.h"
#include "kircholm/kirch_system.h"
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

WeightedUnidirectionalGraph<Map::SiteTypeId, Movability>
MovementSystem::make_weighted_graph(Unit::IdType id) {
    WeightedUnidirectionalGraph graph(map().graph(), 0);

    const auto hex_tab = get_movability_table(units(), id);
    const auto rot_tab = get_rotation_table();

    for (const auto& [node, neighbors] : map().graph().adjacency_matrix()) {
        const auto& [node_id, dir] = node;
        const auto site_type       = map().type_of(node_id);

        switch (site_type) {
            case Map::SiteType::hex: {
                const auto hex_type =
                    static_cast<HexType>(map().hexes().at(node_id).type());

                for (const auto& neighbor : neighbors) {
                    if (neighbor.first == node_id) {
                        graph.change_edge_weight(neighbor, node, rot_tab.at(hex_type));
                    } else {
                        app_assert(
                            neighbor.second == dir,
                            "in kirch system, direction cannot be changed by non-rotation.");
                        const auto op_dir = HexCoordinate::opposite_direction(dir);
                        graph.change_edge_weight({neighbor.first, op_dir},
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

    const auto hostile    = get_player_units(system()->opposite_player_index());
    auto [postions, zone] = system()->organization.get_positions_and_zone(hostile);

    const auto friendly       = get_player_units(system()->current_player_index());
    auto [f_postions, f_zone] = system()->organization.get_positions_and_zone(friendly);

    for (const auto& [u, pos] : f_postions)
        zone.erase(pos);

    _hostile_zone.clear();

    if (unit_can_enter_hostile_zone(id)) {
        for (const auto& [pos, uts] : zone) {
            auto& set         = _hostile_zone[pos];
            const auto pos_id = map().get_hex_id(pos).value();
            for (const auto& u : uts) {
                const auto cmp = units().get_component<PositionComponent>(u);
                app_assert(cmp, "Position component cannot be null.");
                const auto dir = pos.neighbor_direction(cmp->position.value());
                if (dir) {
                    set.insert(*dir);
                }
            }
            for (int i = 0; i < HexCoordinate::neighbors_count(); ++i)
                if (set.count(i) == 0)
                    graph.remove_node({pos_id, i});
        }
    } else {
        for (const auto& [pos, uts] : zone) {
            const auto pos_id = map().get_hex_id(pos).value();
            for (int i = 0; i < HexCoordinate::neighbors_count(); ++i)
                graph.remove_node({pos_id, i});
        }
    }

    return graph;
}

MovementSystem::MovementSystem(SystemKircholm* system) noexcept
    : ComponentSystemKircholm{system} {}

MovementSystem::State MovementSystem::init_movement(HexCoordinate coord) {
    app_info("Initiating MovementSystem.");

    const auto friendly = get_player_units(system()->current_player_index());

    _target_pc = units().find_first<PositionComponent>([&coord, &friendly](auto& cmp) {
        if (cmp.position == coord && friendly.count(cmp.owner()) == 1) {
            return true;
        }
        return false;
    });

    if (!_target_pc) {
        return State::idle;
    }

    _target_mc = units().get_component<MovementComponent>(_target_pc->owner());
    if (!_target_mc) {
        return State::idle;
    }
    if (_target_mc->moving_pts == 0 || _target_mc->immobilized) {
        app_trace("Unit {} is immobilized", _target_mc->owner());
        return State::idle;
    }
    const auto graph = make_weighted_graph(_target_pc->owner());
    _searcher        = PathSearcher(graph);
    _hostile_zone.erase(_target_pc->position.value());

    const auto start_hex = map().get_hex_id(coord);
    engine_assert_throw(start_hex.has_value(),
                        "Initializing movement with nonexistent hex on this map.");

    _searcher.search_path({start_hex.value(), _target_pc->direction.value()},
                          _target_mc->moving_pts);
    return State::paths_calculated;
}

std::vector<std::tuple<HexCoordinate, int, Movability>> MovementSystem::make_path_preview(
    HexCoordinate destination, int dir) const {
    const auto dest_hex = map().get_hex_id(destination);
    if (!dest_hex)
        return {};
    dir = dir % HexCoordinate::neighbors_count();

    const auto ids = _searcher.path_indices({*dest_hex, dir});
    if (ids.empty())
        return {};
    std::vector<std::tuple<HexCoordinate, int, Movability>> res;
    res.reserve(ids.size());
    int prev_id;
    for (const auto& [site, mov] : ids) {
        if (res.size() != 0 && prev_id == site.first)
            continue;
        if (site.first == *dest_hex)
            break;
        res.push_back({map().get_hex_coord(site.first).value(), site.second, mov});
        prev_id = site.first;
    }
    res.push_back({destination, dir, ids.back().second});
    return res;
}

MovementSystem::State MovementSystem::set_target_hex(HexCoordinate hex) {
    _target_hex = hex;
    if (hex == _target_pc->position.value()) {
        _path             = {{hex, _target_pc->direction.value(), 0}};
        _target_direction = _target_pc->direction.value();
        return State::target_hex_set;
    }
    const auto dest_hex = map().get_hex_id(_target_hex);
    if (!dest_hex)
        return State::idle;

    const auto dirs = get_avaliable_dirs();
    if (dirs.empty())
        return State::idle;

    const auto it_min = std::min_element(
        dirs.cbegin(), dirs.cend(),
        [](const auto& val1, const auto& val2) { return val1.second < val2.second; });
    _target_direction = it_min->first;
    _path             = make_path_preview(_target_hex, _target_direction);
    app_assert(!_path.empty(), "");

    if (_hostile_zone.empty())
        return State::target_hex_set;

    const auto it = _hostile_zone.find(hex);
    _immobilized  = it != _hostile_zone.end();
    if (_immobilized) {
        return set_target_dir(*it->second.begin());
    }
    if (dirs.size() == 1)
        return set_target_dir(dirs.begin()->first);

    return State::target_hex_set;
}

std::map<int, Movability> MovementSystem::get_avaliable_dirs() {
    const auto site_id = map().get_hex_id(_target_hex).value();
    std::map<int, Movability> res{};
    for (const auto& [site, dist] : _searcher.distances()) {
        if (site.first != site_id)
            continue;
        res[site.second] = dist;
    }
    return res;
}

MovementSystem::State MovementSystem::set_target_dir(HexCoordinate hex) {
    const auto dir = _target_hex.neighbor_direction(hex);
    if (!dir)
        return State::paths_calculated;

    return set_target_dir(*dir);
}

MovementSystem::State MovementSystem::set_target_dir(int dir) {
    const auto t_dir = dir % HexCoordinate::neighbors_count();
    const auto dirs  = get_avaliable_dirs();
    const auto it    = dirs.find(t_dir);
    if (it == dirs.end())
        return State::paths_calculated;

    _target_direction = it->first;
    _path             = make_path_preview(_target_hex, _target_direction);
    return State::target_dir_set;
}

MovementSystem::State MovementSystem::move() {
    app_debug("Moving through path:");
    for (const auto& [hex, dir, cost] : _path) {
        app_debug("hex: ({}, {}), dir: {}, cost: {}", hex.q(), hex.r(), dir, cost);
    }
    app_debug(" ");

    const auto& [td_coord, td_dir, td_mov] = _path.back();

    auto new_mc = *_target_mc;
    new_mc.moving_pts -= td_mov;
    engine_assert(new_mc.moving_pts >= 0, "Unit {} has negative number of moving pts.",
                  new_mc.owner());
    new_mc.immobilized = _immobilized;
    push_action<ComponentChangeAction<MovementComponent>>(std::move(new_mc));

    auto new_pc      = *_target_pc;
    new_pc.position  = td_coord;
    new_pc.direction = td_dir;
    push_action<ComponentChangeAction<PositionComponent>>(std::move(new_pc));

    return State::idle;
}

PathSearcher::PathSearcher(
    const WeightedUnidirectionalGraph<Map::SiteTypeId, Movability>& graph)
    : _graph{graph} {}

void PathSearcher::search_path(Map::SiteTypeId start, Movability max_cost) {
    std::tie(_distances, _paths) = _graph.dijkstra(start);
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

const std::map<Map::SiteTypeId, Movability>& PathSearcher::distances() const {
    return _distances;
}

const std::map<Map::SiteTypeId, Map::SiteTypeId>& PathSearcher::paths() const {
    return _paths;
}

std::vector<std::pair<Map::SiteTypeId, Movability>> PathSearcher::path_indices(
    Map::SiteTypeId destination) const {
    auto it = _distances.find(destination);
    if (it == _distances.end())
        return {};

    std::vector<std::pair<Map::SiteTypeId, Movability>> res = {{destination, it->second}};

    auto i = _paths.find(destination);
    while (i != _paths.end()) {
        const auto& site = i->second;
        res.push_back({site, _distances.at(site)});
        i = _paths.find(site);
    }

    std::reverse(res.begin(), res.end());
    return res;
}

void MovementSystem::reset_moving_pts() {
    units().apply_for_each<MovementComponent>([](MovementComponent& mc) {
        mc.moving_pts = mc.total_moving_pts();
        return true;
    });
}

void MovementSystem::on_left_click(HexCoordinate coord) {
    switch (_state) {
        case State::idle:
            _state = init_movement(coord);
            break;
        case State::paths_calculated:
            _state = set_target_hex(coord);
            break;
        case State::target_hex_set:
            break;
        case State::target_dir_set:
            break;
        default:
            app_assert(false, "Unimplemented case in MovementSystem::on_left_click.");
    }
}

void MovementSystem::on_right_click(HexCoordinate) {
    switch (_state) {
        case State::idle:
            break;
        case State::paths_calculated:
            _state = State::idle;
            break;
        case State::target_hex_set:
            _state = State::paths_calculated;
            break;
        case State::target_dir_set:
            break;
        default:
            app_assert(false, "Unimplemented case in MovementSystem::on_left_click.");
    }
}

void MovementSystem::on_left_realease(HexCoordinate coord) {
    switch (_state) {
        case State::idle:
            break;
        case State::paths_calculated:
            break;
        case State::target_hex_set:
            _state = set_target_dir(coord);
            break;
        case State::target_dir_set:
            break;
        default:
            app_assert(false, "Unimplemented case in MovementSystem::on_left_click.");
    }
}

void MovementSystem::on_right_realease(HexCoordinate coord) {
    switch (_state) {
        case State::idle:
            break;
        case State::paths_calculated:
            break;
        case State::target_hex_set:
            break;
        case State::target_dir_set:
            break;
        default:
            app_assert(false, "Unimplemented case in MovementSystem::on_left_click.");
    }
}

void MovementSystem::on_over(HexCoordinate coord) {
    switch (_state) {
        case State::idle:
            break;
        case State::paths_calculated:
            set_target_hex(coord);
            break;
        case State::target_hex_set:
            break;
        case State::target_dir_set:
            break;
        default:
            app_assert(false, "Unimplemented case in MovementSystem::on_left_click.");
    }
}

void MovementSystem::update() {
    switch (_state) {
        case State::idle:
            break;
        case State::paths_calculated:
            for (const auto& [hex, dir, mp] : _path)
                system()->gfx.highlight_hex(hex);
            break;
        case State::target_hex_set:
            break;
        case State::target_dir_set:
            _state = move();
            break;
        default:
            app_assert(false, "Unimplemented case in MovementSystem::on_left_click.");
    }
}

bool MovementSystem::unit_can_enter_hostile_zone(Unit::IdType unit) const {
    const auto cmp = units().get_component<MovementComponent>(unit);
    if (cmp == nullptr)
        return false;
    if (cmp->formation == MovementComponent::FormationType::attack ||
        cmp->formation == MovementComponent::FormationType::march)
        return true;

    return false;
}

void MovementSystem::on_init() { _state = State::idle; }

}  // namespace kirch