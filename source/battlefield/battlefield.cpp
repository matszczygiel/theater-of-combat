#include "battlefield.h"

#include <algorithm>

void Battlefield::carry_fight() {
    int pl1_strength = 0;
    for (auto& u : _player1_units)
        pl1_strength += u->get_strength_points();

    int pl2_strength = 0;
    for (auto& u : _player2_units)
        pl2_strength += u->get_strength_points();

    
}