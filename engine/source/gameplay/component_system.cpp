#include "toc/gameplay/component_system.h"

ComponentSystem::ComponentSystem(System* system) noexcept : sys{system} {
    engine_assert(system != nullptr, "Cannot use nullptr as system");
}

UnitManager& ComponentSystem::units() noexcept { return scenario().units; }
Map& ComponentSystem::map() noexcept { return scenario().map; }
Scenario& ComponentSystem::scenario() noexcept { return *sys->scenario; }

const UnitManager& ComponentSystem::units() const noexcept { return scenario().units; }
const Map& ComponentSystem::map() const noexcept { return scenario().map; }
const Scenario& ComponentSystem::scenario() const noexcept { return *sys->scenario; }