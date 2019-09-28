#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <vector>
#include <stack>

#include <cereal/types/array.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/set.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>

#include "action.h"
#include "map/map.h"
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

    void next_day();
    int current_day() const;

    template <class Archive>
    void save(Archive& archive);

    template <class Archive>
    void load(Archive& archive);

   private:
    bool prepare_lua_state() const;

    int _current_day{0};
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
    std::array<std::string, 2> player_names{};

    void start();
    void next_player();
    
    bool set_local_player(std::string name);
    void set_local_player(int index);

    bool is_local_player_now() const;

    int current_player_index() const;
    int opposite_player_index() const;

    template <class Archive>
    void serialize(Archive& archive);

   private:
    friend class UndoPreviousAction;

    void next_phase();

    int _current_player_index{-1};
    std::optional<int> _local_player_index{};

    std::stack<std::unique_ptr<Action>, std::vector<std::unique_ptr<Action>>>
        _action_stack{};
};

template <class Archive>
void GameState::serialize(Archive& archive) {
    archive(CEREAL_NVP(*scenario), CEREAL_NVP(phase), CEREAL_NVP(player_names),
            CEREAL_NVP(_current_player_index), CEREAL_NVP(_action_stack));
}

#endif