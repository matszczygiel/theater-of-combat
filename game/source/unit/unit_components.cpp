#include "unit_components.h"

#include "toc/core/log.h"

MovementComponent::MovementComponent(Movability moving_points)
    : moving_pts{moving_points}, position{}, _total_moving_pts{moving_points} {
    engine_assert_throw(
        moving_points >= 0,
        "MovementComponent should be initialized with positive number ({}) of moving pts.",
        moving_pts);
}

