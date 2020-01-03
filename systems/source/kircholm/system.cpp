#include "kircholm/kirch_system.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/polymorphic.hpp>

#include "kircholm/kirch_gui.h"
#include "kircholm/kirch_lua.h"

namespace kirch {

SystemKircholm::SystemKircholm() : movement{this}, direct_fight{this} {};

void SystemKircholm::start() {
    System::start();
    current_phase = StatePhase::movement;
    app_info("SystemKircholm started. Current phase: movement");
}

void SystemKircholm::next_phase() {
    switch (current_phase) {
        case StatePhase::movement:
            current_phase = StatePhase::bombardment;
            app_info("SystemKircholm changed phase to bombardment");
            break;
        case StatePhase::bombardment:
            current_phase = StatePhase::attack;
            app_info("SystemKircholm changed phase to attack");
            direct_fight.init_direct_fights();
            break;
        case StatePhase::attack:
            current_phase = StatePhase::counterattack;
            app_info("SystemKircholm changed phase to counterattack");
            next_player();
            if (current_player_index() == 0)
                movement.reset_moving_pts();
            break;
        case StatePhase::counterattack:
            current_phase = StatePhase::movement;
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
        switch (current_phase) {
            case StatePhase::movement:
                if (movement.is_moving() && !movement.is_hex_set()) {
                    auto path = movement.path_preview(hex);
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
        switch (current_phase) {
            case StatePhase::movement:
                if (!movement.is_moving()) {
                    movement.init_movement(hex);
                } else {
                    movement.set_target_hex(hex);
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
        debug->unit_info->set_current_unit_id(pc->owner());

    if (is_local_player_now()) {
        switch (current_phase) {
            case StatePhase::movement:
                if (movement.is_moving())
                    movement.reset();
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
        switch (current_phase) {
            case StatePhase::movement:
                if (movement.is_moving() && movement.is_hex_set()) {
                    const auto& target   = std::get<0>(movement.path().back());
                    const auto neighbors = target.neighbors();
                    const auto it = std::find(neighbors.cbegin(), neighbors.cend(), hex);
                    if (it != neighbors.cend()) {
                        const auto direction = std::distance(neighbors.cbegin(), it);
                        movement.set_target_dir(direction);
                    } else if (target == hex) {
                        movement.move();
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
    debug            = std::make_shared<DebugInfoSystem>();
    debug->unit_info = std::make_unique<KircholmUnitInfo>(scenario);
    return debug;
}

void SystemKircholm::update_system() {
    if (current_phase == StatePhase::attack) {
        if (direct_fight.is_done()) {
            push_action(std::make_unique<NextPhaseAction>());
            return;
        }
        direct_fight.process_retreats();
    }
}

}  // namespace kirch

CEREAL_REGISTER_TYPE(kirch::SystemKircholm);