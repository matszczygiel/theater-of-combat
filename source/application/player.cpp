#include "player.h"

void Player::add_unit(Unit* unit) {
    auto insertion_result = _units.insert(unit);
    if (!insertion_result.second) {
        GAME_ERROR("Failed to assign unit to a player: {0}", _name);
        throw std::runtime_error("Failed to assign unit to a player");
    }
}