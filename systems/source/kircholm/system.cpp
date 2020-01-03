#include "kircholm/kirch_system.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/polymorphic.hpp>

#include "kircholm/kirch_gui.h"
#include "kircholm/kirch_lua.h"

namespace kirch {

SystemKircholm::SystemKircholm() : _movement{this}, _direct_fight{this} {};

void SystemKircholm::start() {
    System::start();
    _current_phase = StatePhase::movement;
    app_info("SystemKircholm started. Current phase: movement");
}

void SystemKircholm::next_phase() {
    switch (_current_phase) {
        case StatePhase::movement:
            _current_phase = StatePhase::bombardment;
            app_info("SystemKircholm changed phase to bombardment");
            break;
        case StatePhase::bombardment:
            _current_phase = StatePhase::attack;
            app_info("SystemKircholm changed phase to attack");
            _direct_fight.init_direct_fights();
            break;
        case StatePhase::attack:
            _current_phase = StatePhase::counterattack;
            app_info("SystemKircholm changed phase to counterattack");
            next_player();
            if (current_player_index() == 0)
                _movement.reset_moving_pts();
            break;
        case StatePhase::counterattack:
            _current_phase = StatePhase::movement;
            app_info("SystemKircholm changed phase to movement");
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
                if (_movement.is_moving() && !_movement.is_hex_set()) {
                    auto path = _movement.path_preview(hex);
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

    if (is_local_player_now()) {
        switch (_current_phase) {
            case StatePhase::movement:
                if (!_movement.is_moving()) {
                    _movement.init_movement(hex);
                } else {
                    _movement.set_target_hex(hex);
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
                if (_movement.is_moving())
                    _movement.reset();
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
                if (_movement.is_moving() && _movement.is_hex_set()) {
                    const auto& target   = std::get<0>(_movement.path().back());
                    const auto neighbors = target.neighbors();
                    const auto it = std::find(neighbors.cbegin(), neighbors.cend(), hex);
                    if (it != neighbors.cend()) {
                        const auto direction = std::distance(neighbors.cbegin(), it);
                        _movement.set_target_dir(direction);
                    } else if (target == hex) {
                        _movement.move();
                    }
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

void SystemKircholm::update_system() {
    if (_current_phase == StatePhase::attack) {
        if (_direct_fight.is_done()) {
            push_action(std::make_unique<NextPhaseAction>());
            return;
        }
        _direct_fight.process_retreats();
        const auto res = _direct_fight.fetch_handled_retreat();
        res.has_value();
        if(!res)
            return;
        
    }
}

}  // namespace kirch

CEREAL_REGISTER_TYPE(kirch::SystemKircholm);