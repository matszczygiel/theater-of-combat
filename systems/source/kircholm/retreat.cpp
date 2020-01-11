#include "kircholm/kirch_retreat.h"

#include <imgui.h>

#include "kircholm/kirch_actions.h"
#include "kircholm/kirch_system.h"

namespace kirch {
RetreatSystem::RetreatSystem(SystemKircholm* system) noexcept
    : ComponentSystemKircholm{system} {}

void RetreatSystem::set_results(const std::vector<DirectFightResult>& results) {
    _results     = results;
    _vec_not_set = false;
}

bool RetreatSystem::is_done() { return _attackers_done && _deffenders_done; }

void RetreatSystem::process_retreats() {
    if (system()->is_local_player_now()) {
        if (_attackers_done)
            return;
    } else {
        if (_deffenders_done)
            return;
    }

    if (fetch_retreat())
        return;

    if (!ImGui::Begin("Fight results", nullptr)) {
        ImGui::End();
        return;
    }

    int counter = 0;
    for (const auto& res : _results) {
        ImGui::BulletText("Result %d", counter);
        if (ImGui::Button("Retreat", ImVec2(100, 50))) {
            _current = res;
            _results.erase(_results.begin() + counter);
        }
        ++counter;
        ImGui::Separator();
    }
    ImGui::End();
}

bool RetreatSystem::fetch_retreat() {
    if (!_current) {
        if (_results.empty() && !_vec_not_set)
            push_action<RetreatsDone>(system()->is_local_player_now());
        return false;
    }

    auto& res             = *_current;
    auto process_unit_set = [this](std::set<Unit::IdType>& units) -> Unit::IdType {
        if (units.empty()) {
            _current = {};
        }
        const auto u = *units.begin();
        units.erase(u);
        return u;
    };

    Unit::IdType u;
    do {
        if (res.break_through < 0) {
            u = process_unit_set(res.ids.attacker_units);
        } else {
            u = process_unit_set(res.ids.deffender_units);
        }
        if (!_current)
            return false;
    } while (res.units_destroyed.count(u) != 0);

    const auto cmp = units().get_component<PositionComponent>(u);
    app_assert(cmp, "");
    app_assert(cmp->position.has_value(), "");

    app_info("Initiating retreat for unit {}", u);
    const auto ret = system()->movement.init_movement(cmp->position.value());
    if (ret == MovementSystem::State::idle)
        app_info("Initiating failed");
    return true;
}

void RetreatSystem::prepare() noexcept {
    _attackers_done  = false;
    _deffenders_done = false;
    _vec_not_set     = true;
}

void RetreatSystem::defence_finished() noexcept { _deffenders_done = true; }
void RetreatSystem::attack_finished() noexcept { _attackers_done = true; }

}  // namespace kirch