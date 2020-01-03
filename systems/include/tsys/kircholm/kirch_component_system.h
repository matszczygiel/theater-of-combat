#ifndef KIRCH_COMPONENT_SYSTEM_H
#define KIRCH_COMPONENT_SYSTEM_H

#include "toc/gameplay/component_system.h"

namespace kirch {

class SystemKircholm;

class ComponentSystemKircholm : public ComponentSystem {
   public:
    explicit ComponentSystemKircholm(SystemKircholm* system);

   protected:
    SystemKircholm* system();
    const SystemKircholm* system() const;
};
}  // namespace kirch

#endif /* KIRCH_COMPONENT_SYSTEM_H */
