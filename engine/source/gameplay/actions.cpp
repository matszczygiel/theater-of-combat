#include "gameplay/actions.h"

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/polymorphic.hpp>

#include "core/log.h"
#include "gameplay/system.h"

bool UndoPreviousAction::execute(System* state) {
    engine_assert(!_executed, "UndoPreviousAction executed more than once.");

    auto& actions = state->_action_stack;
    engine_assert(actions.size() > 0,
                  "UndoPreviousAction cannot be executed on empty _action_stack.");
    _executed = actions.top()->revert(state);
    if (!_executed)
        return false;

    _reverted_action = std::move(actions.top());
    actions.pop();
    return true;
}

bool UndoPreviousAction::revert(System* state) {
    app_assert(_executed, "UndoPreviousAction reverted before executed.");
    auto& actions = state->_action_stack;
    _executed     = !_reverted_action->execute(state);
    if (_executed)
        return false;

    actions.push(std::move(_reverted_action));
    _reverted_action = nullptr;
    return true;
}

CEREAL_REGISTER_TYPE(UndoPreviousAction);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Action, UndoPreviousAction);

bool NextPhaseAction::execute(System* state) {
    state->next_phase();
    return true;
}

bool NextPhaseAction::revert(System*) { return false; }

CEREAL_REGISTER_TYPE(NextPhaseAction);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Action, NextPhaseAction);

CEREAL_REGISTER_TYPE(ComponentChangeAction<PositionComponent>);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Action, ComponentChangeAction<PositionComponent>);

UnitDestroyedAction::UnitDestroyedAction(Unit::IdType id) : _id{id} {}

bool UnitDestroyedAction::execute(System* state) {
    state->scenario->units.remove(_id);
    return true;
}

bool UnitDestroyedAction::revert(System*) { return false; }

CEREAL_REGISTER_TYPE(UnitDestroyedAction);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Action, UnitDestroyedAction);
