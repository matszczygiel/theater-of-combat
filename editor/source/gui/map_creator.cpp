#include "map_creator.h"

#include <imgui.h>

#include "toc/gameplay/action.h"

MapCreator::MapCreator(const std::string name) noexcept : _name{name} {}

bool MapCreator::is_ready() const { return _ready; }

void MapCreator::show(bool* open) {
    if (!ImGui::Begin(_name.c_str(), open)) {
        ImGui::End();
        return;
    }

    constexpr ImU16 max_size = 100;
    static ImU16 width;
    static ImU16 height;
    ImGui::InputScalar("width", ImGuiDataType_U16, &width);
    ImGui::InputScalar("height", ImGuiDataType_U16, &height);
    if (width <= 0)
        width = 1;
    else if (width > max_size)
        width = max_size;
    if (height <= 0)
        height = 1;
    else if (height > max_size)
        height = max_size;

    if (ImGui::Button("Create map", ImVec2(100, 40))) {
        _ready  = true;
        *open   = false;
        _height = static_cast<int>(height);
        _width  = static_cast<int>(width);
    }

    ImGui::SameLine();
    if (ImGui::Button("Create test", ImVec2(100, 40))) {
        *open        = false;
        _create_test = true;
        _ready       = true;
    }

    ImGui::SameLine();
    if (ImGui::Button("Exit", ImVec2(100, 40)))
        *open = false;

    ImGui::End();
}

bool MapCreator::retrieve(std::unique_ptr<SystemState>& system) {
    if (!_ready || !system) {
        _ready = false;
        return false;
    }
    _ready = false;
    if (_create_test) {
        system->scenario->map = Map::create_test_map();
        _create_test          = false;
    } else {
        system->scenario->map = Map::create_plain_map(_width, _height);
    }

    return true;
}