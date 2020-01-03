#include "kircholm/kirch_component_system.h"

#include "kircholm/kirch_system.h"

namespace kirch {

ComponentSystemKircholm::ComponentSystemKircholm(SystemKircholm* system)
    : ComponentSystem(system) {}

SystemKircholm* ComponentSystemKircholm::system() {
    return static_cast<SystemKircholm*>(_sys);
}

const SystemKircholm* ComponentSystemKircholm::system() const {
    return static_cast<const SystemKircholm*>(_sys);
}

}  // namespace kirch