#pragma once

#include <vector>

#include "application.h"
#include "map.h"
#include "unit.h"
#include "mover.h"

class Game : public Application {
   private:
    void initialize() override;
    void handle_event(const sf::Event& event) override;
    void update() override;
    void finalize() override;
    void render() override;

    std::vector<Unit*> _units;
    Map _map;
    Mover _mover;


    constexpr static float token_size = 30;
};