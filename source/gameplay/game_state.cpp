#include "game_state.h"

#include "core/log.h"
#include "core/lua_vm.h"

void Scenario::next_day() {
    ++_current_day;
    sol::protected_function daily = lua::get_state()["day"];
    auto res                      = daily(_current_day);
    if (!res.valid()) {
        app_error("Scenario's daily (day {}) lua script failed on load.", _current_day);
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
    app_info("Loading scenario's lua script.");
    try {
        lua::get_state().safe_script(_script, lua::error_handler);
    } catch (sol::error& err) {
        app_error("Scenario's lua script failed on load.");
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
    auto pos = std::find(std::begin(player_names), std::end(player_names), name);
    if (pos != std::end(player_names)) {
        _local_player_index = std::distance(player_names.begin(), pos);
        return true;
    } else {
        return false;
    }
}

void GameState::set_local_player(int index) {
    app_assert(index >= 0 && index < 2, "Invalid player index.");
    _local_player_index = index;
}

void GameState::next_player() {
    app_assert(phase != GamePhase::not_started, "Game not started.");
    if (++_current_player_index == static_cast<int>(player_names.size())) {
        _current_player_index = 0;
    }
}

void GameState::start() {
    app_assert(phase == GamePhase::not_started, "Game already started.");
    app_assert(_local_player_index.has_value(), "Local player not defined.");
    _current_player_index = 0;
    phase                 = GamePhase::movement;
    scenario->next_day();
}

void GameState::next_phase() {
    switch (phase) {
        case GamePhase::movement:
            phase = GamePhase::battles;
            break;
        case GamePhase::battles:
            phase = GamePhase::retreats;
            next_player();
            break;
        case GamePhase::retreats:
            phase = GamePhase::chases;
            next_player();
            break;
        case GamePhase::chases:
            phase = GamePhase::movement;
            next_player();
            if (current_player_index() == 0)
                scenario->next_day();

            break;
        default:
            break;
    }
}

bool GameState::is_local_player_now() const {
    return _local_player_index == _current_player_index;
}

int GameState::current_player_index() const { return _current_player_index; }

int GameState::opposite_player_index() const { return (_current_player_index + 1) % 2; }
