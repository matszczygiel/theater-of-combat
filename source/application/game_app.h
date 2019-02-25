#pragma once

#include <vector>

#include "application.h"
#include "map/map.h"
#include "mover/mover.h"
#include "unit/unit.h"

class Game : public Application {
   private:
    void initialize() override;
    void handle_event(const sf::Event& event) override;
    void update(const sf::Time& elapsed_time) override;
    void finalize() override;
    void render() override;

    std::vector<Unit*> _units;
    Map _map;
    Mover* _mover = nullptr;

    bool _moving = false;

    bool _moving_view_up     = false;
    bool _moving_view_down   = false;
    bool _moving_view_right  = false;
    bool _moving_view_left   = false;

    constexpr static float token_size         = 30;
    constexpr static float _view_moving_speed = 0.3f;
};