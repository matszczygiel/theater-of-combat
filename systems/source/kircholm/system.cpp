#include "kircholm/kirch_system.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/polymorphic.hpp>

#include "kircholm/kirch_gui.h"
#include "kircholm/kirch_lua.h"

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
    gfx.highlight_hex(hex);

    if (is_local_player_now()) {
        switch (_current_phase) {
            case StatePhase::movement:
                if (_movement.is_moving()) {
                    auto path = _movement.path_preview(hex, 0);
                    const auto last =
                        std::unique(path.begin(), path.end(),
                                    [](const auto& site1, const auto& site2) {
                                        return std::get<0>(site1) == std::get<0>(site2);
                                    });
                    path.erase(last, path.end());
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
    gfx.highlight_hex(hex);

    if (is_local_player_now()) {
        switch (_current_phase) {
            case StatePhase::movement:
                if (!_movement.is_moving()) {
                    _movement.init_movement(
                        hex, scenario->player_teams[current_player_index()],
                        scenario->player_teams[opposite_player_index()]);
                    _tmp_move_target = {};
                } else {
                    _tmp_move_target = hex;
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

void SystemKircholm::handle_hex_release(const HexCoordinate& hex) {
    if (is_local_player_now()) {
        switch (_current_phase) {
            case StatePhase::movement:
                if (_movement.is_moving() && _tmp_move_target) {
                    const auto& target   = *_tmp_move_target;
                    const auto neighbors = target.neighbors();
                    const auto it = std::find(neighbors.cbegin(), neighbors.cend(), hex);
                    if (it != neighbors.cend()) {
                        const auto direction = std::distance(neighbors.cbegin(), it);
                        auto actions         = _movement.move_target(target, direction);
                        for (auto& a : actions)
                            push_action(std::move(a));
                    } else if (_tmp_move_target == hex) {
                        auto path       = _movement.path_preview(hex, 0);
                        const auto last = std::unique(
                            path.begin(), path.end(),
                            [](const auto& site1, const auto& site2) {
                                return std::get<0>(site1) == std::get<0>(site2);
                            });
                        path.erase(last, path.end());
                        const auto [h, dir, cost] = path.back();
                        auto actions              = _movement.move_target(h, dir);
                        for (auto& a : actions)
                            push_action(std::move(a));
                    }
                    _tmp_move_target = {};
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

}  // namespace kirch

std::shared_ptr<DebugInfoSystem> SystemKircholm::create_debug_info() {
    _debug            = std::make_shared<DebugInfoSystem>();
    _debug->unit_info = std::make_unique<KircholmUnitInfo>(scenario);
    return _debug;
}

}  // namespace kirch

CEREAL_REGISTER_TYPE(kirch::SystemKircholm);