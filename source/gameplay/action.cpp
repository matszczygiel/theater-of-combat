#include "action.h"

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/polymorphic.hpp>

#include "core/log.h"
#include "game_state.h"

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

template <class Component>
ComponentChangeAction<Component>::ComponentChangeAction(const Component& component)
    : _new_component{component} {}

template <class Component>
void ComponentChangeAction<Component>::execute(GameState* state) {
    app_assert(!_old_component, "ComponentChangeAction<{}> executed more than once.",
               typeid(Component).name());
    auto& unit_man = state->scenario->units;

    const auto& owner = _new_component.owner();
    auto cmp          = unit_man.get_component<Component>(owner);
    app_assert(cmp, "Procesing nonexistent ComponentChangeAction<{}>. Owner: {}.",
               typeid(Component).name(), owner);
    _old_component = *cmp;
    *cmp           = _new_component;
}

template <class Component>
void ComponentChangeAction<Component>::revert(GameState* state) {
    app_assert(_old_component.has_value(),
               "ComponentChangeAction<{}> reverted before executed.",
               typeid(Component).name());
    auto& unit_man = state->scenario->units;

    const auto& owner = _old_component.value().owner();
    auto cmp          = unit_man.get_component<Component>(owner);
    app_assert(cmp, "Procesing nonexistent ComponentChangeAction<{}>. Owner: {}.",
               typeid(Component).name(), owner);
    *cmp = _old_component.value();
    _old_component.reset();
}

template <class Component>
template <class Archive>
void ComponentChangeAction<Component>::serialize(Archive& ar) {
    ar(CEREAL_NVP(_new_component));
}

// List all components
template class ComponentChangeAction<MovementComponent>;

CEREAL_REGISTER_TYPE(ComponentChangeAction<MovementComponent>);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Action, ComponentChangeAction<MovementComponent>);