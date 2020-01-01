#include "debug/debug_info.h"

#include <sstream>

#include <cereal/archives/json.hpp>
#include <cereal/types/memory.hpp>

#include "toc/core/log.h"
#include "toc/gameplay/actions.h"

void DebugInfoSystem::log_action(const std::unique_ptr<Action>& action) const {
    engine_info("Debugging action.");
    engine_debug("Printing its JSON representation.");
    std::stringstream ss;
    {
        cereal::JSONOutputArchive ar(ss);
        ar(action);
    }

    std::string line;
    while (std::getline(ss, line)) {
        engine_debug("JSON: {}", line);
    }
}
