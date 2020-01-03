#include "gameplay/system.h"

#include <memory>

#include "core/log.h"
#include "core/lua_vm.h"
#include "gameplay/actions.h"

void System::start() { _current_player_index = 0; }

void System::push_action(std::unique_ptr<Action> action) {
    if (action)
        accumulated_actions.push_back(std::move(action));
}

bool System::set_local_player(std::string name) {
    auto pos = std::find(std::begin(player_names), std::end(player_names), name);
    if (pos != std::end(player_names)) {
        _local_player_index = std::distance(player_names.begin(), pos);
        return true;
    } else {
        return false;
    }
}

void System::set_local_player(int index) {
    engine_assert(index >= 0 && index < 2, "Invalid player index {}.", index);
    _local_player_index = index;
}

void System::next_player() {
    if (++_current_player_index == static_cast<int>(player_names.size())) {
        _current_player_index = 0;
    }
}

bool System::is_local_player_now() const {
    return _local_player_index == _current_player_index;
}

int System::current_player_index() const { return _current_player_index; }

int System::opposite_player_index() const { return (_current_player_index + 1) % 2; }

void System::update() {
    decltype(accumulated_actions) actions{};
    actions.swap(accumulated_actions);

    for (auto& action : actions) {
        action->execute(this);
        _action_stack.push(std::move(action));
    }

    update_system();
    gfx.update();
}

std::shared_ptr<DebugInfoSystem> System::create_debug_info() {
    auto dbg       = std::make_shared<DebugInfoSystem>();
    dbg->unit_info = std::make_unique<UnitInfo>(scenario);
    return dbg;
}

void System::clear() {}

void System::update_system() {}
