#pragma once

#include <vector>

#include "unit/unit.h"

class Battlefield {
   public:
    void add_unit_player1(Unit* unit);
    void add_unit_player2(Unit* unit);

    void carry_fight();

   private:
    std::vector<Unit*> _player1_units;
    std::vector<Unit*> _player2_units;
};

void Battlefield::add_unit_player1(Unit* unit) { _player1_units.push_back(unit); }
void Battlefield::add_unit_player2(Unit* unit) { _player2_units.push_back(unit); }