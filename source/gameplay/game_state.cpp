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
        local_player_index = std::distance(players.begin(), pos);
        return true;
    } else {
        return false;
    }
}

void GameState::next_player() {
    app_assert(phase != GamePhase::not_started, "Game not started.");
    if (++current_player_index == static_cast<int>(players.size())) {
        current_player_index = 0;
        next_phase();
    }
}

void GameState::start() {
    app_assert(phase == GamePhase::not_started, "Game already started.");
    app_assert(local_player_index.has_value(), "Local player not defined.");
    current_player_index = 0;
    phase                = GamePhase::movement;
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

bool GameState::is_local_player_now() const {
    return local_player_index == current_player_index;
}

const Player& GameState::current_player() const {
    return players[current_player_index];
}

const Player& GameState::opposite_player() const {
    return players[(current_player_index + 1) % 2];
}
