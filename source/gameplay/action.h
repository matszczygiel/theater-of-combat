#ifndef ACTION_H
#define ACTION_H

#include "unit/unit_components.h"

class GameState;

class Action {
   public:
    virtual void execute(GameState* state) = 0;
    virtual void revert(GameState* state)  = 0;

    virtual ~Action() = default;
};

class UndoPreviousAction : public Action {
   public:
    virtual void execute(GameState* state) override;
    virtual void revert(GameState* state) override;

   private:
    bool _executed{false};
    std::unique_ptr<Action> _reverted_action{nullptr};
};

class MovementAction : public Action {
   public:
    MovementAction(const std::vector<MovementComponent>& components);
    
    virtual void execute(GameState* state) override;
    virtual void revert(GameState* state) override;

   private:
    bool _executed{false};
    std::vector<MovementComponent> _components_to_change{};
    std::vector<MovementComponent> _components_before_change{};
};

#endif