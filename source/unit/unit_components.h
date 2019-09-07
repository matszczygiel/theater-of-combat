#ifndef UNIT_COMPONENTS_H
#define UNIT_COMPONENTS_H

#include <optional>

#include "unit.h"
#include "map/hexagons.h"

struct MovementComponent : public ComponentBase {
    MovementComponent() = default;
    MovementComponent(UnitType t, int moving_points);

    const UnitType type{};
    const int total_moving_pts{};
    int moving_pts{};

    std::optional<HexCoordinate> position{};
};

#endif