#include "kircholm/kirch_system.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/polymorphic.hpp>

#include "kircholm/kirch_gui.h"
#include "kircholm/kirch_lua.h"

namespace kirch {

SystemKircholm::SystemKircholm()
    : movement{this},
      direct_fight{this},
      retreat{this},
      organization{this},
      chase{this} {};

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
            direct_fight.on_init();
            app_info("SystemKircholm changed phase to attack");
            break;
        case StatePhase::attack:
            current_phase = StatePhase::chase;
            app_info("SystemKircholm changed phase to chase");
            if (current_player_index() == 0)
                movement.reset_moving_pts();
            break;
        case StatePhase::chase:
            next_player();
            current_phase = StatePhase::movement;
            app_info("SystemKircholm changed phase to movement");
            break;
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
            case StatePhase::chase:
                break;
        }
    }
}

void SystemKircholm::on_left_click(const HexCoordinate& hex) {
    gfx.highlighted_hexes.clear();
    gfx.highlight_hex(hex);

    switch (current_phase) {
        case StatePhase::movement:
            if (is_local_player_now())
                movement.on_left_click(hex);
            break;
        case StatePhase::bombardment:
            break;
        case StatePhase::attack:
            retreat.on_left_click(hex);
            break;
        case StatePhase::chase:
            chase.on_left_click(hex);
            break;
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

    switch (current_phase) {
        case StatePhase::movement:
            if (is_local_player_now()) {
                movement.on_right_click(hex);
            }
            break;
        case StatePhase::bombardment:
            break;
        case StatePhase::attack:
            break;
        case StatePhase::chase:
            chase.on_right_click(hex);
            break;
    }
}

void SystemKircholm::on_left_realease(const HexCoordinate& hex) {
    switch (current_phase) {
        case StatePhase::movement:
            if (is_local_player_now()) {
                movement.on_left_realease(hex);
            }
            break;
        case StatePhase::bombardment:
            break;
        case StatePhase::attack:
            break;
        case StatePhase::chase:
            chase.on_left_realease(hex);
            break;
    }
}

void SystemKircholm::on_right_realease(const HexCoordinate& hex) {
    switch (current_phase) {
        case StatePhase::movement:
            if (is_local_player_now()) {
                movement.on_right_realease(hex);
            }
            break;
        case StatePhase::bombardment:
            break;
        case StatePhase::attack:
            break;
        case StatePhase::chase:
            break;
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
            retreat.update();
            break;
        case StatePhase::chase:
            chase.update();
            break;
        default:
            break;
    }
}

}  // namespace kirch

CEREAL_REGISTER_TYPE(kirch::SystemKircholm);