#ifndef ACTION_H
#define ACTION_H

#include "game_state.h"

class Action {
    public:
     virtual void execute(GameState& state) = 0;
     virtual void revert(GameState& state) = 0;
};

class MovementAction : public Action {
    public:
     virtual void execute(GameState& state) override;
     virtual void revert(GameState& state) override;
    private:
     bool _executed{false};
     std::vector<MovementComponent> _components_to_change{};
     std::vector<MovementComponent> _components_before_change{};
};


#endif