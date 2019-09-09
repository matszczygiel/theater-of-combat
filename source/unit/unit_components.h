#ifndef UNIT_COMPONENTS_H
#define UNIT_COMPONENTS_H

#include <optional>

#include "map/hexagons.h"
#include "unit.h"

struct MovementComponent : public ComponentBase {
    MovementComponent() = default;
    explicit MovementComponent(int moving_points) noexcept;

    int moving_pts{};
    std::optional<HexCoordinate> position{};

    const int& total_moving_pts() const;

   private:
    int _total_moving_pts{};
};

#endif