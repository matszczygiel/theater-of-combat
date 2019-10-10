#include "debug_info.h"

#include <memory>
#include <sstream>

#include <cereal/archives/json.hpp>
#include <cereal/types/memory.hpp>

#include "toc/core/log.h"

namespace debug_info {
DebugInfoSystem::DebugInfoSystem(const GameState& state)
    : _scenario{state.scenario} {}

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
}  // namespace debug_info
