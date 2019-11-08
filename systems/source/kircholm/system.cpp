#include "kircholm/kirch_system.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/polymorphic.hpp>

namespace kirch {

SystemKircholm::SystemKircholm() : _movement{scenario} {};

void SystemKircholm::start() {
    SystemState::start();
    _current_phase = StatePhase::movement;
}

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
            if (current_player_index() == 0) {
                scenario->units.apply_for_each<MovementComponent>(
                    [](MovementComponent& mc) {
                        mc.moving_pts = mc.total_moving_pts();
                        return true;
                    });
            }
            break;
        case StatePhase::counterattack:
            _current_phase = StatePhase::movement;
            break;

        default:
            app_assert(false, "Unknown StatePhase");
    }
}

void SystemKircholm::handle_hex_over(const HexCoordinate& hex) {
    gfx.highlighted_hexes.clear();
    gfx.highlight_hex(hex);

    if (is_local_player_now()) {
        switch (_current_phase) {
            case StatePhase::movement:
                if (_movement.is_moving()) {
                    const auto path = _movement.path_preview(hex);
                    for (const auto h : path)
                        gfx.highlight_hex(h);
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

void SystemKircholm::handle_hex_selection(const HexCoordinate& hex) {
    gfx.highlighted_hexes.clear();
    gfx.highlight_hex(hex);

    if (is_local_player_now()) {
        switch (_current_phase) {
            case StatePhase::movement:
                if (!_movement.is_moving()) {
                    _movement.init_movement(
                        hex, scenario->player_teams[current_player_index()],
                        scenario->player_teams[opposite_player_index()]);
                } else {
                    auto actions = _movement.move_target(hex);
                    for (auto& a : actions)
                        push_action(std::move(a));
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

void SystemKircholm::handle_hex_info(const HexCoordinate& hex) {
    gfx.highlighted_hexes.clear();
    gfx.highlight_hex(hex);

    if (is_local_player_now()) {
        switch (_current_phase) {
            case StatePhase::movement:
                if (_movement.is_moving()) {
                    const auto path = _movement.path_preview(hex);
                    for (const auto h : path)
                        gfx.highlight_hex(h);
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