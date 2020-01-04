#include "kircholm/kirch_retreat.h"

#include <imgui.h>

#include "kircholm/kirch_system.h"

namespace kirch {
RetreatSystem::RetreatSystem(SystemKircholm* system) noexcept
    : ComponentSystemKircholm{system} {}

void RetreatSystem::set_results(const std::vector<DirectFightResult>& results) {
    _results  = results;
    _prepared = false;
}

bool RetreatSystem::is_done() { return _results.empty() && !_current && !_prepared; }

void RetreatSystem::process_retreats() {
    if (fetch_retreat())
        return;
    if (!ImGui::Begin("Fight results", nullptr)) {
        ImGui::End();
        return;
    }

    int counter = 0;
    for (const auto& res : _results) {
        ImGui::BulletText("Result %d", counter);
        if (ImGui::Button("Retreat", ImVec2(200, 100))) {
            _current = res;
            _results.erase(_results.begin() + counter);
            app_assert(fetch_retreat(), "");
        }
        ++counter;
        ImGui::Separator();
    }
    ImGui::End();
}

bool RetreatSystem::fetch_retreat() {
    if (!_current)
        return false;
    if (system()->movement.is_moving())
        return false;

    auto& res = *_current;
    Unit::IdType u;
    if (res.break_through < 0) {
        if (res.ids.attacker_units.empty()) {
            _current = {};
            return false;
        }
        u = *res.ids.attacker_units.begin();
        res.ids.attacker_units.erase(u);
    } else {
        if (res.ids.deffender_units.empty()) {
            _current = {};
            return false;
        }
        u = *res.ids.deffender_units.begin();
        res.ids.deffender_units.erase(u);
    }
    const auto cmp = units().get_component<PositionComponent>(u);
    app_assert(cmp, "");
    app_assert(cmp->position.has_value(), "");

    app_info("Initiating retreat for unit {}", u);
    const auto ret = system()->movement.init_movement(cmp->position.value());
    if (!ret)
        app_info("Initiating failed");
    return true;
}

void RetreatSystem::prepare_for_retreating() noexcept { _prepared = true; }

}  // namespace kirch