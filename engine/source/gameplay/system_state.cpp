#include "gameplay/system_state.h"

#include <memory>

#include "core/log.h"
#include "core/lua_vm.h"
#include "gameplay/action.h"

SystemState::~SystemState() {}

void SystemState::start() { _current_player_index = 0; }

void SystemState::push_action(std::unique_ptr<Action> action) {
    if (action)
        accumulated_actions.push_back(std::move(action));
}

bool SystemState::set_local_player(std::string name) {
    auto pos = std::find(std::begin(player_names), std::end(player_names), name);
    if (pos != std::end(player_names)) {
        _local_player_index = std::distance(player_names.begin(), pos);
        return true;
    } else {
        return false;
    }
}

void SystemState::set_local_player(int index) {
    engine_assert(index >= 0 && index < 2, "Invalid player index {}.", index);
    _local_player_index = index;
}

void SystemState::next_player() {
    if (++_current_player_index == static_cast<int>(player_names.size())) {
        _current_player_index = 0;
    }
}

bool SystemState::is_local_player_now() const {
    return _local_player_index == _current_player_index;
}

int SystemState::current_player_index() const { return _current_player_index; }

int SystemState::opposite_player_index() const { return (_current_player_index + 1) % 2; }

void SystemState::update() {
    for (auto& action : accumulated_actions) {
        action->execute(this);
        _action_stack.push(std::move(action));
    }

    accumulated_actions.clear();

    gfx.update();
}

std::shared_ptr<DebugInfoSystem> SystemState::create_debug_info() {
    auto dbg       = std::make_shared<DebugInfoSystem>();
    dbg->unit_info = std::make_unique<UnitInfo>(scenario);
    return dbg;
}

void SystemState::clear() {}
