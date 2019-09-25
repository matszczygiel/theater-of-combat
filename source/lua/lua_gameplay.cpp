#include "lua_gameplay.h"

#include <sol/sol.hpp>

#include "core/lua_vm.h"
#include "gameplay/game_state.h"

namespace gameplay {
void lua_push_functions() {
    auto& lua                 = lua::get_state();
    auto state                = lua.new_usertype<GameState>("GameState",
                                             sol::constructors<GameState()>());
    state["start"]            = &GameState::start;
    state["set_local_player"] = &GameState::set_local_player;
    state["next_player"]      = &GameState::next_player;
}

}  // namespace gameplay