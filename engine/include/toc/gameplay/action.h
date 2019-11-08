#ifndef ACTION_H
#define ACTION_H

#include <optional>
#include <type_traits>

#include <cereal/macros.hpp>

#include "cereal/optional.hpp"

#include "toc/unit/unit_components.h"
#include "system_state.h"

class Action {
   public:
    virtual bool execute(SystemState* state) = 0;
    virtual bool revert(SystemState* state)  = 0;

    virtual ~Action() = default;
};

class UndoPreviousAction : public Action {
   public:
    virtual bool execute(SystemState* state) override;
    virtual bool revert(SystemState* state) override;

    template <class Archive>
    void serialize(Archive& ar);

   private:
    bool _executed{false};
    std::unique_ptr<Action> _reverted_action{nullptr};
};

template <class Archive>
void UndoPreviousAction::serialize(Archive&) {}

// Client needs to register types with concrete component to cereal
template <class Component>
class ComponentChangeAction : public Action {
    static_assert(std::is_base_of_v<ComponentBase, Component>);

   public:
    ComponentChangeAction() = default;
    ComponentChangeAction(const Component& component);

    virtual bool execute(SystemState* state) override;
    virtual bool revert(SystemState* state) override;

    template <class Archive>
    void serialize(Archive& ar);

   private:
    Component _new_component{};
    std::optional<Component> _old_component{};
};


template <class Component>
ComponentChangeAction<Component>::ComponentChangeAction(const Component& component)
    : _new_component{component} {}

template <class Component>
bool ComponentChangeAction<Component>::execute(SystemState* state) {
    engine_assert(!_old_component, "ComponentChangeAction<{}> executed more than once.",
               typeid(Component).name());
    auto& unit_man = state->scenario->units;

    const auto& owner = _new_component.owner();
    auto cmp          = unit_man.get_component<Component>(owner);
    engine_assert(cmp, "Procesing nonexistent ComponentChangeAction<{}>. Owner: {}.",
               typeid(Component).name(), owner);
    _old_component = *cmp;
    *cmp           = _new_component;
    return true;
}

template <class Component>
bool ComponentChangeAction<Component>::revert(SystemState* state) {
    engine_assert(_old_component.has_value(),
               "ComponentChangeAction<{}> reverted before executed.",
               typeid(Component).name());
    auto& unit_man = state->scenario->units;

    const auto& owner = _old_component.value().owner();
    auto cmp          = unit_man.get_component<Component>(owner);
    engine_assert(cmp, "Procesing nonexistent ComponentChangeAction<{}>. Owner: {}.",
               typeid(Component).name(), owner);
    *cmp = _old_component.value();
    _old_component.reset();
    return true;
}

template <class Component>
template <class Archive>
void ComponentChangeAction<Component>::serialize(Archive& ar) {
    ar(CEREAL_NVP(_new_component));
}
class NextPhaseAction : public Action {
   public:
    virtual bool execute(SystemState* state) override;
    virtual bool revert(SystemState* state) override;

    template <class Archive>
    void serialize(Archive& ar);
};

template <class Archive>
void NextPhaseAction::serialize(Archive&) {}

#endif