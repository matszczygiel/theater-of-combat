#include "gameplay/lua_gameplay.h"

#include <sol/state.hpp>

#include "gameplay/scenario.h"

namespace gameplay {
void lua_push_functions(sol::state& lua) {
    auto scenario =
        lua.new_usertype<Scenario>("Scenario", sol::constructors<Scenario()>());

    scenario["teams"]        = &Scenario::teams;
    scenario["player_teams"] = &Scenario::player_teams;
}
}  