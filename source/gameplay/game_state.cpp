#include "game_state.h"

#include "core/log.h"
#include "core/lua_vm.h"

void Scenario::next_day() {
    ++_current_day;
    sol::protected_function daily = lua::get_state()["day"];
    auto res                      = daily(_current_day);
    if (!res.valid()) {
        app_error("Scenario's daily (day {}) lua script failed on load.",
                  _current_day);
        sol::error err = res;
        app_error("Error message: {}", err.what());
        app_debug("The script that failed\n{}", _script);
    }
}

bool Scenario::load_script(const std::string& script) {
    _script = script;
    if (!prepare_lua_state()) {
        _script.clear();
        return false;
    }
    sol::protected_function initializer = lua::get_state()["init"];
    auto res                            = initializer();
    if (!res.valid()) {
        app_error("Scenario's initializing lua script failed on load.");
        sol::error err = res;
        app_error("Error message: {}", err.what());
        app_debug("The script that failed\n{}", _script);
        _script.clear();
        return false;
    }
    return true;
}

int Scenario::current_day() const { return _current_day; }

bool Scenario::prepare_lua_state() const {
    auto result =
        lua::get_state().safe_script(_script, sol::script_pass_on_error);
    if (!result.valid()) {
        app_error("Scenario's lua script failed on load.");
        sol::error err = result;
        app_error("Error message: {}", err.what());
        app_debug("The script that failed\n{}", _script);
        return false;
    }
    return true;
}

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
            scenario->next_day();
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
