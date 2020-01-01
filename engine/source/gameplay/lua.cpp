#include "gameplay/lua_gameplay.h"

#include <sol/state.hpp>

#include "gameplay/scenario.h"
#include "gameplay/actions.h"
#include "gameplay/system.h"

namespace gameplay {
void lua_push_functions(sol::state& lua) {
    auto scenario =
        lua.new_usertype<Scenario>("Scenario", sol::constructors<Scenario()>());

    scenario["teams"]        = &Scenario::teams;
    scenario["player_teams"] = &Scenario::player_teams;
    scenario["system_name"]       = &Scenario::system;
    scenario["current_turn"] = &Scenario::current_turn;

    auto system =
        lua.new_usertype<System>("System");

    system["set_local_player_name"] =
        sol::resolve<bool(std::string)>(&System::set_local_player);
    system["set_local_player_index"] =
        sol::resolve<void(int)>(&System::set_local_player);
    system["is_local_player_now"]   = &System::is_local_player_now;
    system["current_player_index"]  = &System::current_player_index;
    system["opposite_player_index"] = &System::opposite_player_index;
    system["next_player"]           = &System::next_player;

    system["start"]      = &System::start;
    system["next_phase"] = &System::next_phase;

    system["scenario"]     = &System::scenario;
    system["player_names"] = &System::player_names;
}
}  // namespace gameplay