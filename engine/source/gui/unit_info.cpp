#include "gui/unit_info.h"

#include <imgui.h>

static void show_component_tree(const PositionComponent& pc) {
    if (ImGui::TreeNode("PositionComponent")) {
        if (pc.position)
            ImGui::BulletText("position:         (%d, %d)", pc.position->q(),
                              pc.position->p());
        else
            ImGui::BulletText("position:         none");

        if (pc.direction)
            ImGui::BulletText("direction:        %d", *pc.direction);
        else
            ImGui::BulletText("direction:        none");

        ImGui::TreePop();
    }
}

UnitInfo::UnitInfo(const std::shared_ptr<Scenario> scenario) : _scenario{scenario} {
    engine_assert(scenario != nullptr, "Cannot initialize UnitInfo with nullptr.");
}

void UnitInfo::draw_component_trees() {}

void UnitInfo::set_current_unit_id(Unit::IdType id) {
    _current_unit_id = id;
    _current_unit    = _scenario->units.units().at(_current_unit_id);
    _open            = true;
}

void UnitInfo::show() {
    if (!_open)
        return;

    if (!ImGui::Begin("Unit debug", &_open)) {
        ImGui::End();
        return;
    }

    ImGui::BulletText("Unit id:   %d", _current_unit_id);
    ImGui::BulletText("Unit name: %s", _current_unit.name().c_str());
    ImGui::BulletText("Unit type: %d", _current_unit.type());

    const auto pc = _scenario->units.get_component<PositionComponent>(_current_unit_id);
    if (pc)
        show_component_tree(*pc);

    draw_component_trees();

    ImGui::End();
}
