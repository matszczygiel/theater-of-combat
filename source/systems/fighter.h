#ifndef FIGHTER_H
#define FIGHTER_H

#include <stack>
#include <vector>

#include "gameplay/game_state.h"

struct FightData {
    std::array<std::set<Unit::IdType>, 2> ids{};
    std::set<Map::SiteId> area{};
};

class FightingSystem {
   public:
    explicit FightingSystem(const GameState& state);

    void make_fight_stack(const int attacking_player_index);

   private:
    std::shared_ptr<Scenario> _scenario{nullptr};
    std::vector<FightData> _fight_stack{};
};

#endif