#include "kircholm/kirch_organization.h"

namespace kirch {

OrganizationSystem::OrganizationSystem(SystemKircholm* system) noexcept
    : ComponentSystemKircholm{system} {}

//<<unit, position>, <hex, controlled by units>>
std::pair<std::map<Unit::IdType, HexCoordinate>, OrganizationSystem::ControlZone>
OrganizationSystem::get_positions_and_zone(const std::set<Unit::IdType>& unit_set) const {
    std::map<Unit::IdType, HexCoordinate> positions;
    ControlZone zone;

    units().apply_for_each<PositionComponent>([&unit_set, &positions,
                                               &zone](const auto& cmp) {
        if (unit_set.find(cmp.owner()) != unit_set.cend() && cmp.position.has_value()) {
            const auto pos         = *cmp.position;
            positions[cmp.owner()] = pos;
            zone[pos].insert(cmp.owner());
            const auto neighbors = pos.neighbors();
            for (const auto& n : neighbors)
                zone[n].insert(cmp.owner());
        }
        return true;
    });
    return {positions, zone};
}

}  // namespace kirch