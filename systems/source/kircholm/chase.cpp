#include "kircholm/kirch_chase.h"

#include <imgui.h>

#include "kircholm/kirch_actions.h"
#include "kircholm/kirch_system.h"

namespace kirch {

ChaseSystem::ChaseSystem(SystemKircholm* system) noexcept
    : ComponentSystemKircholm{system} {}

void ChaseSystem::set_results(const std::vector<DirectFightResult>& results) {
    _results = results;

    _fights_to_process.clear();
    _fights_to_wait.clear();
    for (int i = 0; i < static_cast<int>(_results.size()); ++i) {
        const auto& res       = _results[i];
        const bool attack_ret = res.break_through < 0;
        const auto& units_to_check =
            attack_ret ? res.ids.deffender_units : res.ids.attacker_units;
        if (std::all_of(
                units_to_check.cbegin(), units_to_check.cend(),
                [&res](const auto& u) { return res.units_destroyed.count(u) == 1; }))
            continue;
        if (attack_ret != system()->is_local_player_now())
            _fights_to_process.insert(i);
        else
            _fights_to_wait.insert(i);
    }
    _state = State::data_set;
}

void ChaseSystem::on_left_click(HexCoordinate coord) {
    switch (_state) {
        case State::data_selected:
            _state = select_unit(coord);
            break;
        case State::unit_selected:
            _state = unit_selected(coord);
        default:
            break;
    }
}

void ChaseSystem::on_right_click(HexCoordinate coord) {
    switch (_state) {
        case State::unit_selected:
            _state = State::data_selected;
            break;

        default:
            break;
    }
}

void ChaseSystem::on_left_realease(HexCoordinate coord) {
    switch (_state) {
        case State::destination_selected:
            _state = select_direction(coord);
            break;

        default:
            break;
    }
}

void ChaseSystem::update() {
    switch (_state) {
        case State::data_set:
            if (_fights_to_process.empty())
                _state = State::waiting;
            else
                render_ui();
            break;
        case State::data_selected:
            for (const auto& [hex, cmp] : _units_to_move)
                system()->gfx.highlight_hex(hex);

            render_done_button();
            break;
        case State::unit_selected:
            for (const auto& hex : _results.at(_current_data).area)
                system()->gfx.highlight_hex(hex);
            break;
        case State::destination_selected:
            system()->gfx.highlight_hex(_destination);
            break;
        case State::waiting:
            if (_fights_to_wait.empty() && system()->is_local_player_now())
                push_action<NextPhaseAction>();
        default:
            break;
    }
}

void ChaseSystem::render_ui() {
    if (!ImGui::Begin("Select chase", nullptr)) {
        ImGui::End();
        return;
    }

    for (const auto& index : _fights_to_process) {
        ImGui::BulletText("Chase %d", index);
        if (ImGui::Button("Select", ImVec2(100, 50)))
            _state = select_data(index);
        ImGui::Separator();
    }
    ImGui::End();
}

void ChaseSystem::render_done_button() {
    if (!ImGui::Begin("Press when done", nullptr)) {
        ImGui::End();
        return;
    }

    if (ImGui::Button("Done", ImVec2(150, 100))) {
        _state = State::data_set;
        _fights_to_process.erase(_current_data);
        push_action<DirectFightChaseDone>(_current_data);
        if (_fights_to_process.empty())
            _state = State::waiting;
    }

    ImGui::End();
}

ChaseSystem::State ChaseSystem::select_data(int index) {
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

ChaseSystem::State ChaseSystem::select_unit(HexCoordinate coord) {
    const auto it = _units_to_move.find(coord);
    if (it == _units_to_move.cend())
        return State::data_selected;

    _current_pc = it->second;

    if (_results.at(_current_data).area.empty()) {
        app_debug("Destinations are empty for unit: {} in fight_data: {}",
                  _current_pc->owner(), _current_data);
        _units_to_move.erase(coord);
        return State::data_selected;
    }
    return State::unit_selected;
}

ChaseSystem::State ChaseSystem::unit_selected(HexCoordinate coord) {
    if (_results.at(_current_data).area.count(coord) == 0)
        return State::unit_selected;

    _destination = coord;
    return State::destination_selected;
}

ChaseSystem::State ChaseSystem::select_direction(HexCoordinate coord) {
    auto dir = _destination.neighbor_direction(coord);
    if (!dir)
        return State::data_selected;

    auto cmp      = *_current_pc;
    cmp.position  = _destination;
    cmp.direction = *dir;

    push_action<ComponentChangeAction<PositionComponent>>(cmp);
    return State::data_selected;
}

void ChaseSystem::set_chase_done(int index) { _fights_to_wait.erase(index); }

}  // namespace kirch