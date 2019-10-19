#include "gui/components_info.h"

#include <imgui.h>

void show_component_tree(const MovementComponent& mc) {
    if (ImGui::TreeNode("MovementComponent")) {
        ImGui::BulletText("moving pts: %d", mc.moving_pts);
        ImGui::BulletText("total moving pts: %d", mc.total_moving_pts());
        if(mc.position)
        ImGui::BulletText("position: (%d, %d)", mc.position->q(), mc.position->p());
        else
        ImGui::BulletText("position: none");
        
        ImGui::BulletText("immobilized: %d", mc.immobilized);
        ImGui::TreePop();
    }
}

void show_component_tree(const FightComponent& fc) {}