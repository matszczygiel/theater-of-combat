#ifndef ACTIONS_H
#define ACTIONS_H

#include <optional>
#include <type_traits>

#include <cereal/macros.hpp>

#include "cereal/optional.hpp"

#include "system.h"
#include "toc/unit/unit_components.h"

class UndoPreviousAction : public Action {
   public:
    virtual bool execute(System* state) override;
    virtual bool revert(System* state) override;

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

    virtual bool execute(System* state) override;
    virtual bool revert(System* state) override;

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
bool ComponentChangeAction<Component>::execute(System* state) {
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
bool ComponentChangeAction<Component>::revert(System* state) {
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
    virtual bool execute(System* state) override;
    virtual bool revert(System* state) override;

    template <class Archive>
    void serialize(Archive& ar);
};

template <class Archive>
void NextPhaseAction::serialize(Archive&) {}

class UnitDestroyedAction : public Action {
   public:
    UnitDestroyedAction() = default;
    explicit UnitDestroyedAction(Unit::IdType id);

    virtual bool execute(System* state) override;
    virtual bool revert(System* state) override;

    template <class Archive>
    void serialize(Archive& ar);

   private:
    Unit::IdType _id;
};

template <class Archive>
void UnitDestroyedAction::serialize(Archive& ar) {
    ar(CEREAL_NVP(_id));
}

#endif