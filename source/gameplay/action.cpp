#include "action.h"

#include "core/log.h"
#include "game_state.h"

void UndoPreviousAction::execute(GameState* state) {
    app_assert(!_executed, "UndoPreviousAction executed more than once.");

    auto& actions = state->action_stack;
    app_assert(actions.size() > 0,
               "UndoPreviousAction cannot be executed on empty action_stack.");

    _reverted_action = std::move(actions.top());
    actions.pop();
    _reverted_action->revert(state);
    _executed = true;
}

void UndoPreviousAction::revert(GameState* state) {
    app_assert(_executed, "UndoPreviousAction reverted before executed.");
    auto& actions = state->action_stack;
    _reverted_action->execute(state);
    actions.push(std::move(_reverted_action));
    _reverted_action = nullptr;
    _executed        = false;
}

MovementAction::MovementAction(const std::vector<MovementComponent>& components)
    : _components_to_change{components} {}

void MovementAction::execute(GameState* state) {
    app_assert(!_executed, "MovementAction executed more than once.");
    auto& unit_man = *state->scenario.units;

    for(const auto& cmp : _components_to_change) {
        const auto& owner = cmp.owner();
        auto original_cmp = unit_man.get_component<MovementComponent>(owner);
        app_assert(original_cmp,
                   "Procesing nonexistent MovementComponent. Owner: {}.",
                   owner);

        _components_before_change.push_back(*original_cmp);
        *original_cmp = cmp;
    }

    _executed = true;
}

void MovementAction::revert(GameState* state) {
    app_assert(_executed, "MovementAction reverted before executed.");
    auto& unit_man = *state->scenario.units;

    for(const auto& cmp : _components_before_change) {
        const auto& owner = cmp.owner();
        auto changed_cmp = unit_man.get_component<MovementComponent>(owner);
        app_assert(changed_cmp,
                   "Procesing nonexistent MovementComponent. Owner: {}.",
                   owner);
        *changed_cmp = cmp;
    }

    _components_before_change.clear();
    _executed = false;
}
