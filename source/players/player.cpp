#include "player.h"

Player::Player(std::string name, const std::vector<std::string>& teams)
    : _name{name}, _owned_teams{teams} {}