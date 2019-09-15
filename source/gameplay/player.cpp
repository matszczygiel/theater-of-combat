#include "player.h"

Player::Player(std::string name, const std::vector<std::string>& teams)
    : _name{name}, _owned_teams{teams} {}

const std::string& Player::name() const { return _name; }

const std::vector<std::string> Player::teams() const { return _owned_teams; }