#ifndef SCENARIO_H
#define SCENARIO_H

#include <map>
#include <memory>
#include <string>

#include <sol/load_result.hpp>

#include "unit/unit.h"
#include "players/player.h"

class Map;
class UnitManager;

class Scenario {
   public:
    std::map<std::string, std::set<Unit::IdType>> teams{};
    std::shared_ptr<UnitManager> units{nullptr};
    std::shared_ptr<Map> map{nullptr};
    int current_day{1};
    std::map<int, sol::load_result> daily_scripts{};
};

enum class GamePhase {
    not_started,
    movement,
};

class GameState {
    public:
     Scenario scenario{};
     std::array<std::unique_ptr<Player>, 2> players = {nullptr, nullptr};
     decltype(players)::iterator current_player = players.begin();
     decltype(players)::iterator remote_player{};
     decltype(players)::iterator local_player{};
     GamePhase phase{GamePhase::not_started};
};

#endif