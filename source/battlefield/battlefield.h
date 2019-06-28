#pragma once

#include <map>
#include <set>

#include "unit/unit.h"

class Battlefield {
   public:
    void push(Unit* unit, const std::string& owner_name);

    void carry_fight();

   private:
    std::map<std::string, std::set<Unit*>> _bucket;
};

inline void Battlefield::add_unit_player1(Unit* unit) { _player1_units.push_back(unit); }
inline void Battlefield::add_unit_player2(Unit* unit) { _player2_units.push_back(unit); }