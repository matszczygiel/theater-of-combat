#include "action.h"

#include <cereal/archives/binary.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

#include "core/log.h"
#include "game_state.h"


/*
void ActionQueue::push(std::unique_ptr<Action> action) {
    std::lock_guard<std::mutex> lock{_m};
    _actions.push_back(std::move(action));
}

void ActionQueue::process_actions(GameState* state) {
    std::lock_guard<std::mutex> lock{_m};
    for(auto& act : _actions)
        state->push_action(std::move(act));

    _actions.clear();
}
*/
void UndoPreviousAction::execute(GameState* state) {
    app_assert(!_executed, "UndoPreviousAction executed more than once.");

    auto& actions = state->_action_stack;
    app_assert(actions.size() > 0,
               "UndoPreviousAction cannot be executed on empty _action_stack.");

    _reverted_action = std::move(actions.top());
    actions.pop();
    _reverted_action->revert(state);
    _executed = true;
}

void UndoPreviousAction::revert(GameState* state) {
    app_assert(_executed, "UndoPreviousAction reverted before executed.");
    auto& actions = state->_action_stack;
    _reverted_action->execute(state);
    actions.push(std::move(_reverted_action));
    _reverted_action = nullptr;
    _executed        = false;
}

CEREAL_REGISTER_TYPE(UndoPreviousAction);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Action, UndoPreviousAction);

MovementAction::MovementAction(const MovementComponent& component)
    : _new_component{component} {}

void MovementAction::execute(GameState* state) {
    app_assert(!_old_component, "MovementAction executed more than once.");
    auto& unit_man = *state->scenario.units;

    const auto& owner = _new_component.owner();
    auto cmp          = unit_man.get_component<MovementComponent>(owner);
    app_assert(cmp, "Procesing nonexistent MovementComponent. Owner: {}.",
               owner);
    _old_component = *cmp;
    *cmp           = _new_component;
}

void MovementAction::revert(GameState* state) {
    app_assert(_old_component.has_value(),
               "MovementAction reverted before executed.");
    auto& unit_man = *state->scenario.units;

    const auto& owner = _old_component.value().owner();
    auto cmp          = unit_man.get_component<MovementComponent>(owner);
    app_assert(cmp, "Procesing nonexistent MovementComponent. Owner: {}.",
               owner);
    *cmp = _old_component.value();
    _old_component.reset();
}

CEREAL_REGISTER_TYPE(MovementAction);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Action, MovementAction);
