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
    UnitManager units{};
    Map map{};
    std::array<std::vector<std::string>, 2> player_teams{};

    void next_day();
    int current_day() const;

   private:
    int _current_day{1};
    std::map<int, sol::load_result> _daily_scripts{};
};

enum class GamePhase {
    not_started,
    movement,
    battles,
};

class GameState {
   public:
    void push_action(std::unique_ptr<Action> action);

    std::shared_ptr<Scenario> scenario{nullptr};
    GamePhase phase{GamePhase::not_started};
    std::array<Player, 2> players{};

    void start();
    bool set_local_player(std::string name);

    void next_player();

   private:
    friend class UndoPreviousAction;

    void next_phase();

    decltype(players)::iterator local_player{};
    decltype(players)::iterator current_player{};

    std::stack<std::unique_ptr<Action>, std::vector<std::unique_ptr<Action>>>
        _action_stack{};
};

#endif