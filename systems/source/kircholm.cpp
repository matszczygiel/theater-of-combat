#include "kircholm.h"

#include <cereal/types/polymorphic.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>

namespace kirch {
MovementComponent::MovementComponent(Movability moving_points)
    : moving_pts{moving_points}, _total_moving_pts{moving_points} {
    engine_assert_throw(
        moving_points >= 0,
        "MovementComponent should be initialized with positive number ({}) of moving pts.",
        moving_pts);
}

SystemKircholm::SystemKircholm() : _movement{this->scenario} {};

void SystemKircholm::start() {}
void SystemKircholm::next_phase() {}

CEREAL_REGISTER_TYPE(SystemKircholm);



CEREAL_REGISTER_TYPE(ComponentChangeAction<MovementComponent>);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Action, ComponentChangeAction<MovementComponent>);

CEREAL_REGISTER_TYPE(ComponentChangeAction<FightComponent>);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Action, ComponentChangeAction<FightComponent>);
}  // namespace kirch