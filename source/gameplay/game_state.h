#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include <sol/load_result.hpp>

#include "action.h"
#include "map/map.h"
#include "player.h"
#include "unit/unit.h"
#include "unit/unit_manager.h"

class Scenario {
   public:
    std::map<std::string, std::set<Unit::IdType>> teams{};
    std::shared_ptr<UnitManager> units{std::make_shared<UnitManager>()};
    std::shared_ptr<Map> map{std::make_shared<Map>()};

    void next_day();
    int current_day() const;

   private:
    int _current_day{1};
    std::map<int, sol::load_result> _daily_scripts{};
};

enum class GamePhase {
    not_started,
    movement,
};

class GameState {
   public:
    void push_action(std::unique_ptr<Action> action);

    Scenario scenario{};
    GamePhase phase{GamePhase::not_started};
    std::array<Player, 2> players{};
    decltype(players)::iterator local_player{};
    decltype(players)::iterator current_player = players.begin();

   private:
    friend class UndoPreviousAction;

    std::stack<std::unique_ptr<Action>, std::vector<std::unique_ptr<Action>>>
        _action_stack{};
};

#endif