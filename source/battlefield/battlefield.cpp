#include "battlefield.h"

#include <algorithm>
#include <random>

#include "randomizer.h"
#include "log.h"

void Battlefield::carry_fight() {
/*
    int pl1_strength = 0;
    for (auto& u : _player1_units)
        pl1_strength += u->get_strength_points();

    int pl2_strength = 0;
    for (auto& u : _player2_units)
        pl2_strength += u->get_strength_points();

    std::uniform_int_distribution<> pl1_distribution(0, pl1_strength / 10);
    std::uniform_int_distribution<> pl2_distribution(0, pl2_strength / 10);

    auto pl2_loss = pl1_distribution(Randomizer::get_engine());
    auto pl1_loss = pl2_distribution(Randomizer::get_engine());

    GAME_INFO("Player1 lost {0} pts.", pl1_loss);
    GAME_INFO("Player2 lost {0} pts.", pl2_loss);
    */
}