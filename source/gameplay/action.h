#ifndef ACTION_H
#define ACTION_H

#include <optional>
#include <thread>

#include "unit/unit_components.h"

class GameState;

class Action {
   public:
    virtual void execute(GameState* state) = 0;
    virtual void revert(GameState* state)  = 0;

    virtual ~Action() = default;
};

class ActionQueue {
    public:
     void push(std::unique_ptr<Action> action);

     void process_actions(GameState* state);

     private:
      std::mutex _m;
      std::vector<std::unique_ptr<Action>> _actions{};
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
    MovementAction(const MovementComponent& component);
    
    virtual void execute(GameState* state) override;
    virtual void revert(GameState* state) override;

   private:
    MovementComponent _new_component{};
    std::optional<MovementComponent> _old_component{};
};

#endif