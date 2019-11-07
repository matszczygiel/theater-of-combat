#include "kircholm.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/polymorphic.hpp>

#include "toc/gameplay/action.h"

namespace kirch {
MovementComponent::MovementComponent(Movability moving_points)
    : moving_pts{moving_points}, _total_moving_pts{moving_points} {
    engine_assert_throw(
        moving_points >= 0,
        "MovementComponent should be initialized with positive number ({}) of moving pts.",
        moving_pts);
}

SystemKircholm::SystemKircholm() /*: _movement{this->scenario}*/ {};

void SystemKircholm::start() {}
void SystemKircholm::next_phase() {}
}  // namespace kirch

CEREAL_REGISTER_TYPE(kirch::SystemKircholm);

CEREAL_REGISTER_TYPE(ComponentChangeAction<kirch::MovementComponent>);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Action, ComponentChangeAction<kirch::MovementComponent>);

CEREAL_REGISTER_TYPE(ComponentChangeAction<kirch::DirectFightComponent>);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Action, ComponentChangeAction<kirch::DirectFightComponent>);
