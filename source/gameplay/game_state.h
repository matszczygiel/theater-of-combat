#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include <cereal/types/array.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/set.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>

#include "action.h"
#include "map/map.h"
#include "player.h"
#include "unit/unit.h"
#include "unit/unit_manager.h"
#include "core/log.h"


class Scenario {
   public:
    std::map<std::string, std::set<Unit::IdType>> teams{};
    UnitManager units{};
    Map map{};
    std::array<std::vector<std::string>, 2> player_teams{};

    bool load_script(const std::string& script);

    void initialize();
    void next_day();
    int current_day() const;

    template <class Archive>
    void save(Archive& archive);

    template <class Archive>
    void load(Archive& archive);

   private:
    bool prepare_lua_state() const;

    int _current_day{1};
    std::string _script{};
};

template <class Archive>
void Scenario::save(Archive& archive) {
    archive(CEREAL_NVP(teams), CEREAL_NVP(units), CEREAL_NVP(map),
            CEREAL_NVP(player_teams), CEREAL_NVP(_current_day),
            CEREAL_NVP(_script));
}


template <class Archive>
void Scenario::load(Archive& archive) {
    archive(CEREAL_NVP(teams), CEREAL_NVP(units), CEREAL_NVP(map),
            CEREAL_NVP(player_teams), CEREAL_NVP(_current_day),
            CEREAL_NVP(_script));

    prepare_lua_state();
}

enum class GamePhase {
    not_started,
    movement,
    battles,
};

class GameState {
   public:
    void push_action(std::unique_ptr<Action> action);

    std::shared_ptr<Scenario> scenario{std::make_shared<Scenario>()};
    GamePhase phase{GamePhase::not_started};
    std::array<Player, 2> players{};

    void start();
    bool set_local_player(std::string name);

    void next_player();

    bool is_local_player_now() const;

    const Player& current_player() const;
    const Player& opposite_player() const;

    template <class Archive>
    void serialize(Archive& archive);

   private:
    friend class UndoPreviousAction;

    void next_phase();

    int current_player_index{-1};
    std::optional<int> local_player_index{};

    std::stack<std::unique_ptr<Action>, std::vector<std::unique_ptr<Action>>>
        _action_stack{};
};

template <class Archive>
void GameState::serialize(Archive& archive) {
    archive(CEREAL_NVP(*scenario), CEREAL_NVP(phase), CEREAL_NVP(players),
            CEREAL_NVP(current_player_index), CEREAL_NVP(_action_stack));
}

#endif