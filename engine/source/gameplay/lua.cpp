#include "gameplay/lua_gameplay.h"

#include <sol/state.hpp>

#include "gameplay/scenario.h"
#include "gameplay/action.h"
#include "gameplay/system_state.h"

namespace gameplay {
void lua_push_functions(sol::state& lua) {
    auto scenario =
        lua.new_usertype<Scenario>("Scenario", sol::constructors<Scenario()>());

    scenario["teams"]        = &Scenario::teams;
    scenario["player_teams"] = &Scenario::player_teams;
    scenario["system_name"]       = &Scenario::system;
    scenario["current_turn"] = &Scenario::current_turn;

    auto system =
        lua.new_usertype<SystemState>("SystemState");

    system["set_local_player_name"] =
        sol::resolve<bool(std::string)>(&SystemState::set_local_player);
    system["set_local_player_index"] =
        sol::resolve<void(int)>(&SystemState::set_local_player);
    system["is_local_player_now"]   = &SystemState::is_local_player_now;
    system["current_player_index"]  = &SystemState::current_player_index;
    system["opposite_player_index"] = &SystemState::opposite_player_index;
    system["next_player"]           = &SystemState::next_player;

    system["start"]      = &SystemState::start;
    system["next_phase"] = &SystemState::next_phase;

    system["scenario"]     = &SystemState::scenario;
    system["player_names"] = &SystemState::player_names;
}
}  // namespace gameplay