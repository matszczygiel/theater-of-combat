#ifndef ACTION_H
#define ACTION_H

#include <optional>
#include <type_traits>

#include <cereal/macros.hpp>

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

    template <class Archive>
    void serialize(Archive& ar);

   private:
    bool _executed{false};
    std::unique_ptr<Action> _reverted_action{nullptr};
};

template <class Archive>
void UndoPreviousAction::serialize(Archive&) {}

class MovementAction : public Action {
   public:
    MovementAction() = default;
    MovementAction(const MovementComponent& component);

    virtual void execute(GameState* state) override;
    virtual void revert(GameState* state) override;

    template <class Archive>
    void serialize(Archive& ar);

   private:
    MovementComponent _new_component{};
    std::optional<MovementComponent> _old_component{};
};

template <class Archive>
void MovementAction::serialize(Archive& ar) {
    ar(CEREAL_NVP(_new_component));
}

template <class Component>
class ComponentChangeAction : public Action {
    static_assert(std::is_base_of_v<ComponentBase, Component>);

   public:
    ComponentChangeAction() = default;
    ComponentChangeAction(const Component& component);

    virtual void execute(GameState* state) override;
    virtual void revert(GameState* state) override;

    template <class Archive>
    void serialize(Archive& ar);

   private:
    Component _new_component{};
    std::optional<Component> _old_component{};
};
/*
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
*/
#endif