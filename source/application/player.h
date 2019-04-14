#pragma once

#include <set>
#include <string>

#include "unit/unit.h"

class Player {
   public:
    std::set<Unit*>& get_players_units();
    void set_name(const std::string& name);
    void add_unit(Unit* unit);

   private:
    std::string _name;
    std::set<Unit*> _units;
};

inline std::set<Unit*>& Player::get_players_units() { return _units; }
inline void Player::set_name(const std::string& name) { _name = name; }