#include "kircholm/kirch_system.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/polymorphic.hpp>

#include "kircholm/kirch_gui.h"
#include "kircholm/kirch_lua.h"

namespace kirch {

SystemKircholm::SystemKircholm()
    : movement{this}, direct_fight{this}, retreat{this}, organization{this} {};

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
            if (is_local_player_now())
                direct_fight.init_direct_fights();
            retreat.prepare();
            app_info("SystemKircholm changed phase to attack");
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

void SystemKircholm::on_over(const HexCoordinate& hex) {
    gfx.highlighted_hexes.clear();
    gfx.highlight_hex(hex);

    if (is_local_player_now()) {
        switch (current_phase) {
            case StatePhase::movement:
                movement.on_over(hex);
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

void SystemKircholm::on_left_click(const HexCoordinate& hex) {
    gfx.highlighted_hexes.clear();
    gfx.highlight_hex(hex);

    if (is_local_player_now()) {
        switch (current_phase) {
            case StatePhase::movement:
                movement.on_left_click(hex);
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

void SystemKircholm::on_right_click(const HexCoordinate& hex) {
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
                movement.on_right_click(hex);
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

void SystemKircholm::on_left_realease(const HexCoordinate& hex) {
    if (is_local_player_now()) {
        switch (current_phase) {
            case StatePhase::movement:
                movement.on_left_realease(hex);
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

void SystemKircholm::on_right_realease(const HexCoordinate& hex) {
    if (is_local_player_now()) {
        switch (current_phase) {
            case StatePhase::movement:
                movement.on_right_realease(hex);
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
    debug            = std::make_shared<DebugInfoSystem>();
    debug->unit_info = std::make_unique<KircholmUnitInfo>(scenario);
    return debug;
}

void SystemKircholm::update_system() {
    switch (current_phase) {
        case StatePhase::movement:
            movement.update();
            break;
        case StatePhase::bombardment:
            break;
        case StatePhase::attack:
            if (retreat.is_done() && is_local_player_now()) {
                push_action(std::make_unique<NextPhaseAction>());
            }
            break;
        case StatePhase::counterattack:
            break;

        default:
            app_assert(false, "Unknown StatePhase");
    }
}

}  // namespace kirch

CEREAL_REGISTER_TYPE(kirch::SystemKircholm);