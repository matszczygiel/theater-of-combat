#include "unit_components.h"

#include "core/log.h"

MovementComponent::MovementComponent(int moving_points) noexcept
    : moving_pts{moving_points}, position{}, _total_moving_pts{moving_points} {
    app_assert(
        moving_points >= 0,
        "MovementComponent should be initialized with positive number of moving pts.");
}

const int& MovementComponent::total_moving_pts() const {
    return _total_moving_pts;
}

FightComponent::FightComponent(int strength_points) noexcept
    : strength_pts{strength_points} {}
