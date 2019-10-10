#include "lua_gameplay.h"

#include <sol/sol.hpp>

#include "toc/core/lua_vm.h"
#include "gameplay/game_state.h"

namespace gameplay {
void lua_push_functions() {
    auto& lua                 = lua::get_state();
    auto state                = lua.new_usertype<GameState>("GameState",
                                             sol::constructors<GameState()>());
    state["start"]            = &GameState::start;
    state["next_phase"]      = &GameState::next_phase;

    auto scenario                = lua.new_usertype<Scenario>("Scenario",
                                             sol::constructors<Scenario()>());

    scenario["teams"] = &Scenario::teams;
    scenario["player_teams"] = &Scenario::player_teams;
}

}  // namespace gameplay