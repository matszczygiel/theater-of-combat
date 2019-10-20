#include "debug_info.h"

#include <memory>
#include <sstream>

#include <imgui.h>
#include <cereal/archives/json.hpp>
#include <cereal/types/memory.hpp>

#include "toc/core/log.h"
#include "toc/gui/unit_info.h"
#include "toc/unit/unit_components.h"

namespace debug_info {
DebugInfoSystem::DebugInfoSystem(const GameState& state) : _scenario{state.scenario} {}

void DebugInfoSystem::debug_action(const std::unique_ptr<Action>& action) const {
    app_info("Debugging action.");
    app_debug("Printing its JSON representation.");
    std::stringstream ss;
    {
        cereal::JSONOutputArchive ar(ss);
        ar(action);
    }

    std::string line;
    while (std::getline(ss, line)) {
        app_debug("JSON: {}", line);
    }
}

void DebugInfoSystem::debug_unit() {
    if (!_show_unit_info)
        return;
    show_unit_info(_current_unit, _current_unit_id, _scenario->units, &_show_unit_info);
}

void DebugInfoSystem::set_current_unit_position(HexCoordinate coord) {
    MovementComponent* mc{nullptr};
    _scenario->units.apply_for_each<MovementComponent>([&coord, &mc](auto& cmp) {
        if (cmp.position == coord) {
            mc = std::addressof(cmp);
            return false;
        }
        return true;
    });
    if (mc) {
        _current_unit_id = mc->owner();
        _current_unit    = _scenario->units.units().at(_current_unit_id);
        _show_unit_info  = true;
    }
}

}  // namespace debug_info
