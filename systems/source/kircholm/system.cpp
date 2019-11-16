#include "kircholm/kirch_system.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/polymorphic.hpp>

#include "kircholm/kirch_lua.h"
#include "kircholm/kirch_gui.h"

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

void SystemKircholm::prepare_lua(sol::state& lua) { lua_push_functions(lua); }

void SystemKircholm::handle_hex_over(const HexCoordinate& hex) {
    gfx.highlighted_hexes.clear();
    gfx.highlight_hex(hex);

    if (is_local_player_now()) {
        switch (_current_phase) {
            case StatePhase::movement:
                if (_movement.is_moving()) {
                    const auto path = _movement.path_preview(hex, 0);
                    for (const auto& [h, dir, cost] : path)
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

    static bool choosing_dir = false;
    static HexCoordinate target_hex;

    if (is_local_player_now()) {
        switch (_current_phase) {
            case StatePhase::movement:
                if (!_movement.is_moving()) {
                    _movement.init_movement(
                        hex, scenario->player_teams[current_player_index()],
                        scenario->player_teams[opposite_player_index()]);
                } else if (!choosing_dir) {
                    choosing_dir = true;
                    target_hex   = hex;
                } else {
                    const auto neighbors = target_hex.neighbors();
                    const auto it = std::find(neighbors.cbegin(), neighbors.cend(), hex);
                    if (it != neighbors.cend()) {
                        auto actions = _movement.move_target(
                            target_hex, std::distance(neighbors.cbegin(), it));
                        for (auto& a : actions)
                            push_action(std::move(a));
                    }
                    choosing_dir = false;
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

    const PositionComponent* pc{nullptr};
    scenario->units.apply_for_each<PositionComponent>([&hex, &pc](const auto& cmp) {
        if (cmp.position == hex) {
            pc = std::addressof(cmp);
            return false;
        }
        return true;
    });
    if (pc)
        _debug->unit_info->set_current_unit_id(pc->owner());

    if (is_local_player_now()) {
        switch (_current_phase) {
            case StatePhase::movement:
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

std::shared_ptr<DebugInfoSystem> SystemKircholm::create_debug_info() {
    _debug            = std::make_shared<DebugInfoSystem>();
    _debug->unit_info = std::make_unique<KircholmUnitInfo>(scenario);
    return _debug;
}

}  // namespace kirch

CEREAL_REGISTER_TYPE(kirch::SystemKircholm);