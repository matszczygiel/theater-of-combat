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
        action->execute(this);
        _action_stack.push(std::move(action));
    }
}

bool GameState::set_local_player(std::string name) {
    app_assert(phase == GamePhase::not_started, "Game already started.");
    auto pos = std::find_if(std::begin(players), std::end(players),
                            [&](auto& p) { return p.name() == name; });
    if (pos != std::end(players)) {
        local_player = pos;
        return true;
    } else {
        return false;
    }
}

void GameState::next_player() {
    app_assert(phase != GamePhase::not_started, "Game not started.");
    current_player++;
    if (current_player == players.end()) {
        current_player = players.begin();
        next_phase();
    }
}

void GameState::start() {
    app_assert(phase == GamePhase::not_started, "Game already started.");
    current_player = players.begin();
    phase          = GamePhase::movement;
}

void GameState::next_phase() {
    switch (phase) {
        case GamePhase::movement:
            phase = GamePhase::battles;
            break;
        case GamePhase::battles:
            phase = GamePhase::movement;
            break;

        default:
            break;
    }
}
