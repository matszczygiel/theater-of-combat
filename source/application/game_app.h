#pragma once

#include <vector>

#include "application.h"
#include "map/map.h"
#include "unit/unit.h"
#include "mover/mover.h"

class Game : public Application {
   private:
    void initialize() override;
    void handle_event(const sf::Event& event) override;
    void update() override;
    void finalize() override;
    void render() override;

    std::vector<Unit*> _units;
    Map _map;
    Mover* _mover = nullptr;


    constexpr static float token_size = 30;

    bool _moving = false;
};