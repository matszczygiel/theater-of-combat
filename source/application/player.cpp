#include "player.h"

void Player::set_name(const std::string& name) {
    _name = name;
}

const std::string& Player::name() const {
    return _name;
}
