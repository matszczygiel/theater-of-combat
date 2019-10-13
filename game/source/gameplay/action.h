#ifndef ACTION_H
#define ACTION_H

#include <optional>
#include <type_traits>

#include <cereal/macros.hpp>

#include "unit/unit_components.h"

class GameState;

class Action {
   public:
    virtual bool execute(GameState* state) = 0;
    virtual bool revert(GameState* state)  = 0;

    virtual ~Action() = default;
};

class UndoPreviousAction : public Action {
   public:
    virtual bool execute(GameState* state) override;
    virtual bool revert(GameState* state) override;

    template <class Archive>
    void serialize(Archive& ar);

   private:
    bool _executed{false};
    std::unique_ptr<Action> _reverted_action{nullptr};
};

template <class Archive>
void UndoPreviousAction::serialize(Archive&) {}

template <class Component>
class ComponentChangeAction : public Action {
    static_assert(std::is_base_of_v<ComponentBase, Component>);

   public:
    ComponentChangeAction() = default;
    ComponentChangeAction(const Component& component);

    virtual bool execute(GameState* state) override;
    virtual bool revert(GameState* state) override;

    template <class Archive>
    void serialize(Archive& ar);

   private:
    Component _new_component{};
    std::optional<Component> _old_component{};
};

class NextPhaseAction : public Action {
   public:
    virtual bool execute(GameState* state) override;
    virtual bool revert(GameState* state) override;

    template <class Archive>
    void serialize(Archive& ar);
};

template <class Archive>
void NextPhaseAction::serialize(Archive&) {}

#endif