#ifndef COMPONENT_SYSTEM_H
#define COMPONENT_SYSTEM_H

#include <set>

#include "system.h"

class ComponentSystem {
   public:
    explicit ComponentSystem(System* sys) noexcept;

   protected:
    UnitManager& units() noexcept;
    Map& map() noexcept;
    Scenario& scenario() noexcept;

    const UnitManager& units() const noexcept;
    const Map& map() const noexcept;
    const Scenario& scenario() const noexcept;

    std::set<Unit::IdType> get_player_units(int player_index) const;

    template <class Action, typename... Args>
    void push_action(Args&&... args);

    System* _sys{nullptr};
};

template <class ComponentAction, typename... Args>
void ComponentSystem::push_action(Args&&... args) {
    static_assert(std::is_base_of_v<Action, ComponentAction>);
    _sys->push_action(std::make_unique<ComponentAction>(std::forward<Args>(args)...));
}

#endif /* COMPONENT_SYSTEM_H */
