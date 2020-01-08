#ifndef KIRCH_ORGANIZATION_H
#define KIRCH_ORGANIZATION_H

#include "kirch_component_system.h"

namespace kirch {

class OrganizationSystem : public ComponentSystemKircholm {
   public:
    using ControlZone = std::map<HexCoordinate, std::set<Unit::IdType>>;

    explicit OrganizationSystem(SystemKircholm* system) noexcept;

    //<<unit, positions>, <hex, controlled by units>>
    std::pair<std::map<Unit::IdType, HexCoordinate>, ControlZone> get_positions_and_zone(
        const std::set<Unit::IdType>& unit_set) const;
};

}  // namespace kirch
#endif /* KIRCH_ORGANIZATION_H */
