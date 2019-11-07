#include "gui/unit_info.h"
/* TODO
#include <imgui.h>

static void show_component_tree(const MovementComponent& mc) {
    if (ImGui::TreeNode("MovementComponent")) {
        ImGui::BulletText("moving pts:       %d", mc.moving_pts);
        ImGui::BulletText("total moving pts: %d", mc.total_moving_pts());
        if (mc.position)
            ImGui::BulletText("position:         (%d, %d)", mc.position->q(),
                              mc.position->p());
        else
            ImGui::BulletText("position:         none");

        ImGui::BulletText("immobilized:      %d", mc.immobilized);
        ImGui::TreePop();
    }
}

static void show_component_tree(const FightComponent& fc) {
    if (ImGui::TreeNode("FightComponent")) {
        ImGui::BulletText("strength pts: %d", fc.strength_pts);
        ImGui::BulletText("in_fight:     %d", fc.in_fight);
        ImGui::TreePop();
    }
}

void show_unit_info(const Unit& unit, const Unit::IdType& id, const UnitManager& um,
                    bool* open) {
    if (!ImGui::Begin("Unit debug", open)) {
        ImGui::End();
        return;
    }

    ImGui::BulletText("Unit id:   %d", id);
    ImGui::BulletText("Unit name: %s", unit.name().c_str());
    ImGui::BulletText("Unit type: %d", static_cast<int>(unit.type()));

    const auto mc = um.get_component<MovementComponent>(id);
    if (mc)
        show_component_tree(*mc);
    const auto fc = um.get_component<FightComponent>(id);
    if (mc)
        show_component_tree(*fc);

    ImGui::End();
}
*/
