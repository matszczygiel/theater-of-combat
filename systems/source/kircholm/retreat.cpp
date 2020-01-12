#include "kircholm/kirch_retreat.h"

#include <imgui.h>

#include "kircholm/kirch_actions.h"
#include "kircholm/kirch_system.h"

namespace kirch {
RetreatSystem::RetreatSystem(SystemKircholm* system) noexcept
    : ComponentSystemKircholm{system} {}

void RetreatSystem::set_results(const std::vector<DirectFightResult>& results) {
    _results = results;

    _fights_to_process.clear();
    _fights_to_wait.clear();
    for (int i = 0; i < static_cast<int>(_results.size()); ++i) {
        const bool attack_ret = _results[i].break_through < 0;
        if (attack_ret == system()->is_local_player_now())
            _fights_to_process.insert(i);
        else
            _fights_to_wait.insert(i);
    }

    _state = State::data_set;

    if (_fights_to_process.empty())
        _state = State::waiting;
}

void RetreatSystem::on_left_click(HexCoordinate coord) {
    switch (_state) {
        case State::data_selected:
            _state = select_unit_to_retreat(coord);
            break;
        case State::unit_selected:
            _state = select_unit(coord);
        default:
            break;
    }
}

void RetreatSystem::on_right_click(HexCoordinate coord) {}

void RetreatSystem::on_over(HexCoordinate coord) {}

void RetreatSystem::on_left_realease(HexCoordinate coord) {}

void RetreatSystem::on_right_realease(HexCoordinate coord) {}

void RetreatSystem::update() {
    switch (_state) {
        case State::data_set:
            render_ui();
            break;
        case State::data_selected:
            for (const auto& [hex, cmp] : _units_to_move)
                system()->gfx.highlight_hex(hex);
            break;
        case State::unit_selected:
            for (const auto& hex : _destinations)
                system()->gfx.highlight_hex(hex);
            break;
        case State::waiting:
            if (_fights_to_wait.empty()) {
                if (system()->is_local_player_now()) {
                    push_action<RetretsPerformed>(_results);
                    push_action<NextPhaseAction>();
                }
                _state = State::done;
            }
        default:
            break;
    }
}

void RetreatSystem::render_ui() {
    if (!ImGui::Begin("Fight results", nullptr)) {
        ImGui::End();
        return;
    }

    for (const auto& index : _fights_to_process) {
        ImGui::BulletText("Result %d", index);
        if (ImGui::Button("Select", ImVec2(100, 50))) {
            _state = select_data(index);
            break;
        }
        ImGui::Separator();
    }
    ImGui::End();
}

WeightedUnidirectionalGraph<Map::SiteTypeId, Movability>
RetreatSystem::make_weighted_graph() const {
    WeightedUnidirectionalGraph graph(map().graph(), 0);

    auto hostile  = get_player_units(system()->opposite_player_index());
    auto friendly = get_player_units(system()->current_player_index());
    if (!system()->is_local_player_now())
        hostile.swap(friendly);

    const auto [positions, zone] = system()->organization.get_positions_and_zone(hostile);
    std::set<Map::SiteId> to_remove{};

    for (const auto& [pos, uts] : zone) {
        to_remove.insert(map().get_hex_id(pos).value());
    }

    for (const auto& [u, pos] : positions) {
        to_remove.insert(map().get_hex_id(pos).value());
    }

    for (const auto& hex : _results.at(_current_data).area) {
        to_remove.erase(map().get_hex_id(hex).value());
    }

    for (const auto& pos : to_remove) {
        for (int i = 0; i < HexCoordinate::neighbors_count(); ++i)
            graph.remove_node({pos, i});
    }

    for (const auto& [node, neighbors] : graph.adjacency_matrix()) {
        const auto& [node_id, dir] = node;
        for (const auto& neighbor : neighbors) {
            if (neighbor.first.first != node_id) {
                graph.change_edge_weight(node, neighbor.first, 1);
            }
        }
    }

    return graph;
}

RetreatSystem::State RetreatSystem::select_data(int index) {
    _current_data = index;

    _units_to_move.clear();

    auto make_unit_map = [this](const std::set<Unit::IdType>& unit_set) {
        for (const auto& u : unit_set) {
            if (_results.at(_current_data).units_destroyed.count(u) == 1)
                continue;
            const auto cmp = units().get_component<PositionComponent>(u);
            app_assert(cmp != nullptr, "");
            _units_to_move[cmp->position.value()] = cmp;
        }
    };

    if (system()->is_local_player_now())
        make_unit_map(_results.at(_current_data).ids.attacker_units);
    else
        make_unit_map(_results.at(_current_data).ids.deffender_units);

    return State::data_selected;
}

RetreatSystem::State RetreatSystem::select_unit_to_retreat(HexCoordinate coord) {
    const auto it = _units_to_move.find(coord);
    if (it == _units_to_move.cend())
        return State::data_selected;

    _current_pc       = it->second;
    _searcher         = PathSearcher(make_weighted_graph());
    const auto hex_id = map().get_hex_id(coord).value();
    const auto range  = std::abs(_results.at(_current_data).break_through);
    _searcher.search_path({hex_id, _current_pc->direction.value()}, range);
    _destinations.clear();
    for (const auto& [site, dist] : _searcher.distances()) {
        if (dist != range)
            continue;
        _destinations.insert(map().get_hex_coord(site.first).value());
    }

    if (_destinations.empty()) {
        app_debug("Destinations are empty for unit: {} in fight_data: {}",
                  _current_pc->owner(), _current_data);
        _units_to_move.erase(coord);
        return State::data_selected;
    }
    return State::unit_selected;
}

RetreatSystem::State RetreatSystem::select_unit(HexCoordinate coord) {
    if (_destinations.count(coord) == 0)
        return State::unit_selected;

    const auto hex_id = map().get_hex_id(coord).value();
    const auto it =
        std::find_if(_searcher.distances().cbegin(), _searcher.distances().cend(),
                     [&hex_id](const auto& val) { return val.first.first == hex_id; });
    app_assert(it != _searcher.distances().cend(), "");
    const auto path = _searcher.path_indices(it->first);
    app_assert(!path.empty(), "");

    auto cmp = *_current_pc;

    Map::SiteTypeId last = path.front().first;
    for (int i = 1; i < static_cast<int>(path.size()); ++i) {
        if (last.first == path[i].first.first)
            continue;
        last = path[i].first;
        if (last.first == hex_id) {
            cmp.direction = HexCoordinate::opposite_direction(last.second);
            cmp.position  = coord;
            break;
        }

        const auto hex = map().get_hex_coord(last.first).value();
        _results.at(_current_data).area.insert(hex);
    }
    _units_to_move.erase(_current_pc->position.value());
    push_action<ComponentChangeAction<PositionComponent>>(cmp);
    if (_units_to_move.empty()) {
        _fights_to_process.erase(_current_data);
        push_action<DirectFightResultChanged>(_results, _current_data);

        if (_fights_to_process.empty())
            return State::waiting;
        else
            return State::data_set;
    }

    return State::data_selected;
}

void RetreatSystem::set_result_change(const DirectFightResult& result, int index) {
    _results.at(index) = result;
    _fights_to_wait.erase(index);
}

}  // namespace kirch