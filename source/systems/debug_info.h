#ifndef DEBUG_INFO_H
#define DEBUG_INFO_H

#include <memory>

#include "gameplay/game_state.h"

class DebugInfoSystem {
    public:
     explicit DebugInfoSystem(GameState& state);

    
    private:
     std::shared_ptr<Map> _map{nullptr};
     std::shared_ptr<UnitManager> _units{nullptr};
};

#endif