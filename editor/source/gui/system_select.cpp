#include "system_select.h"

#include <imgui.h>

#include "toc/gameplay/actions.h"

#include "tsys/systems.h"

SystemSelector::SystemSelector(const std::string name) noexcept : _name{name} {}

void SystemSelector::show(bool* open) {
    constexpr auto list = tsys::list_available_systems();
    if (!ImGui::Begin(_name.c_str(), open, ImGuiWindowFlags_NoScrollbar)) {
        ImGui::End();
        return;
    }
    {
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
        ImGui::BeginChild("Selector list",
                          ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 260), false,
                          ImGuiWindowFlags_HorizontalScrollbar);
        for (int i = 0; i < static_cast<int>(list.size()); ++i) {
            if (ImGui::Selectable(list[i].data(), _selected == i))
                _selected = i;
        }
        ImGui::EndChild();
        ImGui::PopStyleVar();
    }
    ImGui::Separator();

    if (ImGui::Button("Select", ImVec2(100, 40))) {
        _ready = true;
        *open  = false;
    }

    ImGui::SameLine();
    if (ImGui::Button("Exit", ImVec2(100, 40)))
        *open = false;

    ImGui::End();
}

bool SystemSelector::is_ready() const { return _ready; }

bool SystemSelector::retrieve(std::unique_ptr<System>& system) {
    if (!_ready) {
        return false;
    }
    _ready                   = false;
    constexpr auto list      = tsys::list_available_systems();
    const auto& name         = list.at(_selected);
    system                   = tsys::create_system(name);
    system->scenario->system = name;
    return true;
}