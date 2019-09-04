#include "unit_components.h"

#include "core/log.h"

MovementComponent::MovementComponent(UnitType t, int moving_points)
    : type{t},
      total_moving_pts{moving_points},
      moving_pts{moving_points},
      position{} {
    app_assert(
        moving_points >= 0,
        "MovementComponent should be initialized with positive number of moving pts.");
}