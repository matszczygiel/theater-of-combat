#include "game_state.h"

#include "core/log.h"
#include "core/lua_vm.h"

void Scenario::next_turn(sol::state& lua) {
    sol::protected_function turn_fun = lua["turn"];
    auto res                         = turn_fun(++_current_day);
    if (!res.valid()) {
        engine_error("Scenario's turn_fun (turn {}) lua script failed on load.",
                     _current_turn);
        sol::error err = res;
        engine_error("Error message: {}", err.what());
        engine_debug("The script that failed\n{}", _script);
    }
}

bool Scenario::load_script(sol::state& lua, const std::string& script) {
    _script = script;
    if (!prepare_lua_state(lua)) {
        _script.clear();
        return false;
    }
    sol::protected_function initializer = lua["init"];
    auto res                            = initializer();
    if (!res.valid()) {
        engine_error("Scenario's initializing lua script failed on load.");
        sol::error err = res;
        engine_error("Error message: {}", err.what());
        engine_debug("The script that failed\n{}", _script);
        _script.clear();
        return false;
    }
    return true;
}

int Scenario::current_turn() const { return _current_turn; }

bool Scenario::prepare_lua_state(sol::state& lua) const {
    engine_info("Loading scenario's lua script.");
    try {
        lua.safe_script(_script, lua::error_handler);
    } catch (sol::error& err) {
        engine_error("Scenario's lua script failed on load.");
        engine_debug("The script that failed\n{}", _script);
        return false;
    }

    return true;
}
