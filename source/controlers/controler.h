#ifndef CONTROLER_H
#define CONTROLER_H

#include <vector>
#include <memory>

#include "gameplay/action.h"

class GameState;

class Controller {
   public:
    virtual void process_actions(GameState* state) = 0;
};

class LocalController : public Controller {
   public:
    void process_actions(GameState* state) final;

    void push_action()

   private:
    std::vector<std::unique_ptr<Action>> _action_queue{};
};

#endif