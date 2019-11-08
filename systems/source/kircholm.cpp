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

SystemKircholm::SystemKircholm() : _movement{scenario} {};

void SystemKircholm::start() { _current_phase = StatePhase::movement; }

void SystemKircholm::next_phase() {
    switch (_current_phase) {
        case StatePhase::movement:
            _current_phase = StatePhase::bombardment;
            break;
        case StatePhase::bombardment:
            _current_phase = StatePhase::attack;
            break;
        case StatePhase::attack:
            _current_phase = StatePhase::counterattack;
            next_player();
            break;
        case StatePhase::counterattack:
            _current_phase = StatePhase::movement;
            break;

        default:
            app_assert(false, "Unknown StatePhase");
    }
}

void SystemKircholm::handle_hex_event(const HexCoordinate& hex,
                                      SystemState::HexEvent ev) {
    if (is_local_player_now()) {
        switch (_current_phase) {
            case StatePhase::movement:
                switch (ev) {
                    case SystemState::HexEvent::selection:
                        if (!_movement.is_moving()) {
                            _movement.init_movement(
                                hex, scenario->player_teams[current_player_index()],
                                scenario->player_teams[opposite_player_index()]);
                        } else {
                            auto actions = _movement.move_target(hex);
                            for (auto& a : actions)
                                _accumulated_actions.push_back(std::move(a));
                        }
                        break;
                    case SystemState::HexEvent::mouse_over:
                        break;
                    case SystemState::HexEvent::info_request:
                        break;
                    default:
                        break;
                }
                break;
            case StatePhase::bombardment:
                break;
            case StatePhase::attack:
                break;
            case StatePhase::counterattack:
                break;

            default:
                app_assert(false, "Unknown StatePhase");
        }
    }
}

}  // namespace kirch

CEREAL_REGISTER_TYPE(kirch::SystemKircholm);

CEREAL_REGISTER_TYPE(ComponentChangeAction<kirch::MovementComponent>);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Action,
                                     ComponentChangeAction<kirch::MovementComponent>);

CEREAL_REGISTER_TYPE(ComponentChangeAction<kirch::DirectFightComponent>);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Action,
                                     ComponentChangeAction<kirch::DirectFightComponent>);
