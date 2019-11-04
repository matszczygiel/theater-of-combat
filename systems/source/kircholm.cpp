#include "kircholm.h"

namespace kirch {
MovementComponent::MovementComponent(Movability moving_points)
    : moving_pts{moving_points}, position{}, _total_moving_pts{moving_points} {
    engine_assert_throw(
        moving_points >= 0,
        "MovementComponent should be initialized with positive number ({}) of moving pts.",
        moving_pts);
}
}  // namespace kirch