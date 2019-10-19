#ifndef DEBUG_INFO_H
#define DEBUG_INFO_H

#include <memory>

#include "gameplay/game_state.h"
#include "graphics/graphics_state.h"

namespace debug_info {

class DebugInfoSystem {
   public:
    explicit DebugInfoSystem(const GameState& state);

    void debug_action(const std::unique_ptr<Action>& action) const;

    void debug_unit();
    void set_current_unit_position(HexCoordinate coord);

   private:
    const std::shared_ptr<Scenario> _scenario{nullptr};
    Unit::IdType _current_unit_id{};
    Unit _current_unit{};
    bool _show_unit_info{false};
};
}  // namespace debug_info

#endif