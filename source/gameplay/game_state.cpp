#include "game_state.h"

void Scenario::next_day() {
    ++_current_day;
    if (auto it = _daily_scripts.find(_current_day);
        it != _daily_scripts.end()) {
        it->second();
    }
}

int Scenario::current_day() const { return _current_day; }

void GameState::push_action(std::unique_ptr<Action> action) {
    if (action) {
        _action_stack.push(std::move(action));
        _action_stack.top()->execute(this);
    }
}
