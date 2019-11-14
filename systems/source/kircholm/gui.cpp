#include "kircholm/kirch_gui.h"

#include <imgui.h>

#include "kircholm/kirch_components.h"

namespace kirch {
static void show_component_tree(const MovementComponent& mc) {
    if (ImGui::TreeNode("MovementComponent")) {
        ImGui::BulletText("moving_pts:       %d", mc.moving_pts);
        ImGui::BulletText("immobilized:      %d", mc.immobilized);
        ImGui::BulletText("formation:        %d", static_cast<int>(mc.formation));

        ImGui::TreePop();
    }
}

KircholmUnitInfo::KircholmUnitInfo(const std::shared_ptr<Scenario>& scenario)
    : UnitInfo(scenario) {}

void KircholmUnitInfo::draw_component_trees() {
    const auto mc = _scenario->units.get_component<MovementComponent>(_current_unit_id);
    if (mc)
        show_component_tree(*mc);
}
}  // namespace kirch