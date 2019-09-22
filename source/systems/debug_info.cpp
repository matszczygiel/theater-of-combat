#include "debug_info.h"

DebugInfoSystem::DebugInfoSystem(GameState& state)
    : _map{state.scenario.map}, _units{state.scenario.units} {}


