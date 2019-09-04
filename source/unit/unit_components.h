#ifndef UNIT_COMPONENTS_H
#define UNIT_COMPONENTS_H

#include <optional>

#include "map/hexagons.h"
#include "unit.h"

struct MovementComponent {
    MovementComponent(UnitType t, int moving_points);

    const UnitType type;
    const int total_moving_pts;
    int moving_pts;

    std::optional<HexCoordinate> position;
};

#endif