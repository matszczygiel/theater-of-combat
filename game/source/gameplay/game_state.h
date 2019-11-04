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

#include "toc/gameplay/scenario.h"
#include "toc/core/log.h"

#include "action.h"


enum class GamePhase {
    not_started,
    movement,
    battles,
    retreats,
    chases,
    new_day,
};

class GameState {
   public:
    void push_action(std::unique_ptr<Action> action);

    std::shared_ptr<Scenario> scenario{std::make_shared<Scenario>()};
    GamePhase phase{GamePhase::not_started};
    std::array<std::string, 2> player_names{};

    void start();
    void next_phase();
    
    bool set_local_player(std::string name);
    void set_local_player(int index);

    bool is_local_player_now() const;

    int current_player_index() const;
    int opposite_player_index() const;

    template <class Archive>
    void serialize(Archive& archive);

   private:
    friend class UndoPreviousAction;

    void next_player();

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